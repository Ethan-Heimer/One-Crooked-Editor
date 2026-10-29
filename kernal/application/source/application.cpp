#include "application.hpp"
#include "ringbuffer.hpp"
#include "thread"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <thread>
#include <unistd.h>
#include <vector>

#include "subprocess.hpp"
#include "memoryusage.h"

using namespace CrookedEditor::Application;
using namespace Process;
using namespace Queues;

// use compositon to clean this up. this class has way to meny responsabilities
// maybe application should be an orcrestraiter class to different sub responsibilities?

struct Application::Impl{
    public: 
        Impl() : log("debug.log"){}

        ~Impl(){
            log << "in destructor" << std::endl;
            
            for(auto& pair : threadQuitTokens){
                log << "signaling " << pair.first << " to quit" << std::endl;
                threadQuitTokens[pair.first] = true;
            }

            for(auto& pair : threads){
                log << "Killing: " << pair.first << std::endl;
                if(pair.second.joinable())
                    pair.second.join(); 
                log << "Killed" << pair.first << std::endl;
            }

            for(auto& pair : subprocesses){
                pair.second.Kill();
            }

            log << "Closing Log" << std::endl;
            log.close();
        }

        void CleanUp(){
            ThreadHandle tHandle;
            while(deadThreads.Pop(tHandle)){
                PrintLog(std::format("Dead Thread {} Found.", tHandle));
                KillThread(tHandle);
            }

            for(auto& [pid, process] : subprocesses){
                if(!process.IsAlive()){
                    PrintLog(std::format("Dead Subprocess {} Found.", pid));
                    deadProcesses.push(pid);
                }
            }

            SubprocessHandle sHandle;
            while (!deadProcesses.empty()) {
                sHandle = deadProcesses.front(); 
                deadProcesses.pop();
    
                Kill(sHandle);
            }
        }

        ThreadHandle SpawnThread(const ThreadConstructor& threadConstructor){
            int thisThreadsId = nextThreadID;
            threadQuitTokens[thisThreadsId] = false;

            auto onDoneExecuting = [&, thisThreadsId](){
                deadThreads.Push(thisThreadsId);
            };

            threads[thisThreadsId] = threadConstructor.CreateThread(onDoneExecuting, 
                    threadQuitTokens.at(thisThreadsId)); 

            nextThreadID++;

            return thisThreadsId;
        }
            
        SubprocessInfo SpawnSubprocess(const SubprocessConstructor& processConstructor){
            Subprocess subprocess = processConstructor.CreateSubprocess();
            
            Process::FileDescripter inputFd = subprocess.input;
            Process::FileDescripter outputFd = subprocess.output;

            subprocesses[lastSubprocessID] = std::move(subprocess);
            lastSubprocessID++;

            return {lastSubprocessID-1, inputFd, outputFd};
        }

        FileDescripter GetInputFileDescriptor(SubprocessHandle handle) const{
            if(!subprocesses.contains(handle))
                return -1;

            const Subprocess& subprocess = subprocesses.at(handle);
            return subprocess.input;
        }

        FileDescripter GetOutputFileDescriptor(SubprocessHandle handle) const{
            if(!subprocesses.contains(handle))
                return -1;

            const Subprocess& subprocess = subprocesses.at(handle);
            return subprocess.input;
        }

        void Kill(SubprocessHandle handle){
            subprocesses.erase(handle);
        }

        void KillThread(ThreadHandle handle){
            if(!threadQuitTokens.contains(handle))
                return;

            if(threadQuitTokens[handle])
                return;

            threadQuitTokens[handle] = true;

            if(threads[handle].joinable())
                threads[handle].join();

            threads.erase(handle);
            threadQuitTokens.erase(handle);
        }
            
        int GetThreadCount(){
            return threads.size();
        }

        bool DoesThreadExist(ThreadHandle handle){
            return threads.contains(handle);
        }

        std::vector<int> GetThreadIds(){
            std:std::vector<int> ids;
            ids.reserve(threads.size());

            for(auto& [id, thread] : threads){
                ids.push_back(id);
            }

            return std::move(ids);
        }

        MemoryUsageData MemoryUsage(){
            double vmUsage, residentSet;
            Diagnostics::process_mem_usage(vmUsage, residentSet);

            return {vmUsage, residentSet};
        }

        void PrintLog(std::string_view message){
            log << message.data() << std::endl;
        }

        void Quit(){
            quitApplication = true;
        }

    private:
        std::ofstream log{};
        std::map<int, std::atomic<bool>> threadQuitTokens{};
        std::map<int, std::thread> threads{};

        std::map<int, Subprocess> subprocesses{};

        int nextThreadID{};
        int lastSubprocessID{};

        std::atomic<bool> quitApplication{};

        RingBuffer<ThreadHandle, 65536> deadThreads;
        std::queue<SubprocessHandle> deadProcesses;
};

Application::Application() : pImpl(std::make_unique<Impl>()){}
Application::~Application(){
    pImpl.reset();
}

void Application::CleanUp(){
    pImpl->CleanUp();
}

void Application::PrintLog(std::string_view message){
    pImpl->PrintLog(message);
}

ThreadHandle Application::SpawnThread(ThreadConstructor threadConstructor) const {
    return pImpl->SpawnThread(threadConstructor);
}

SubprocessInfo Application::SpawnSubprocess(SubprocessConstructor process) const {
    return pImpl->SpawnSubprocess(process);
}

FileDescripter Application::GetInputFileDescriptor(SubprocessHandle handle) const{
    return pImpl->GetInputFileDescriptor(handle);
}

FileDescripter Application::GetOutputFileDescriptor(SubprocessHandle handle) const{
    return pImpl->GetOutputFileDescriptor(handle);
}

int Application::GetThreadCount(){
    return pImpl->GetThreadCount();
}

std::vector<int> Application::GetThreadIds(){
    return pImpl->GetThreadIds();
}

void Application::Kill(SubprocessHandle handle){
    pImpl->Kill(handle);
}

void Application::KillThread(ThreadHandle handle){
    pImpl->KillThread(handle);
}

bool Application::DoesThreadExist(ThreadHandle handle){
    return pImpl->DoesThreadExist(handle);
};

MemoryUsageData Application::MemoryUsage(){
    return pImpl->MemoryUsage();
};

void Application::Quit(){
    pImpl->Quit();
    OnQuit.Invoke();
}
