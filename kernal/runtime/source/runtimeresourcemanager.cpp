#include "runtimeresourcemanager.hpp"
#include "requestbus.hpp"
#include "ringbuffer.hpp"
#include "thread"
#include <cstddef>
#include <map>
#include <memory>
#include <queue>
#include <thread>
#include <unistd.h>
#include <vector>

#include "subprocess.hpp"
#include "memoryusage.h"

using namespace CrookedEditor::Runtime;
using namespace Process;
using namespace Queues;

struct RuntimeResourceManager::Impl{
    public: 
        Impl(Events::RequestBus& requestBus){}

        ~Impl(){
            for(auto& pair : threadQuitTokens){
                threadQuitTokens[pair.first] = true;
            }

            for(auto& pair : threads){
                if(pair.second.joinable())
                    pair.second.join(); 
            }

            for(auto& pair : subprocesses){
                pair.second.Kill();
            }
        }

        void CleanUp(){
            API::ThreadHandle tHandle;
            while(deadThreads.Pop(tHandle)){
                KillThread(tHandle);
            }

            for(auto& [pid, process] : subprocesses){
                if(!process.IsAlive()){
                    deadProcesses.push(pid);
                }
            }

            API::SubprocessHandle sHandle;
            while (!deadProcesses.empty()) {
                sHandle = deadProcesses.front(); 
                deadProcesses.pop();
    
                Kill(sHandle);
            }
        }

        API::ThreadHandle SpawnThread(const ThreadConstructor& threadConstructor){
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
            
        API::SubprocessInfo SpawnSubprocess(const SubprocessConstructor& processConstructor){
            Subprocess subprocess = processConstructor.CreateSubprocess();
            
            Process::FileDescripter inputFd = subprocess.input;
            Process::FileDescripter outputFd = subprocess.output;

            subprocesses[lastSubprocessID] = std::move(subprocess);
            lastSubprocessID++;

            return {lastSubprocessID-1, inputFd, outputFd};
        }

        FileDescripter GetInputFileDescriptor(API::SubprocessHandle handle) const{
            if(!subprocesses.contains(handle))
                return -1;

            const Subprocess& subprocess = subprocesses.at(handle);
            return subprocess.input;
        }

        FileDescripter GetOutputFileDescriptor(API::SubprocessHandle handle) const{
            if(!subprocesses.contains(handle))
                return -1;

            const Subprocess& subprocess = subprocesses.at(handle);
            return subprocess.input;
        }

        void Kill(API::SubprocessHandle handle){
            subprocesses.erase(handle);
        }

        void KillThread(API::ThreadHandle handle){
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

        bool DoesThreadExist(API::ThreadHandle handle){
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

        API::MemoryUsageData MemoryUsage(){
            double vmUsage, residentSet;
            Diagnostics::process_mem_usage(vmUsage, residentSet);

            return {vmUsage, residentSet};
        }

        void Quit(){
            quit = true;
        }

    private:
        std::map<int, std::atomic<bool>> threadQuitTokens{};
        std::map<int, std::thread> threads{};

        std::map<int, Subprocess> subprocesses{};

        int nextThreadID{};
        int lastSubprocessID{};

        std::atomic<bool> quit{};

        RingBuffer<API::ThreadHandle, 65536> deadThreads;
        std::queue<API::SubprocessHandle> deadProcesses;
};

RuntimeResourceManager::RuntimeResourceManager(Events::RequestBus& requestBus) 
    : pImpl(std::make_unique<Impl>(requestBus)){}
RuntimeResourceManager::~RuntimeResourceManager(){
    pImpl.reset();
}

void RuntimeResourceManager::CleanUp(){
    pImpl->CleanUp();
}

CrookedEditor::API::ThreadHandle RuntimeResourceManager::SpawnThread(ThreadConstructor threadConstructor) const {
    return pImpl->SpawnThread(threadConstructor);
}

CrookedEditor::API::SubprocessInfo RuntimeResourceManager::SpawnSubprocess(SubprocessConstructor process) const {
    return pImpl->SpawnSubprocess(process);
}

FileDescripter RuntimeResourceManager::GetInputFileDescriptor(CrookedEditor::API::SubprocessHandle handle) const{
    return pImpl->GetInputFileDescriptor(handle);
}

FileDescripter RuntimeResourceManager::GetOutputFileDescriptor(CrookedEditor::API::SubprocessHandle handle) const{
    return pImpl->GetOutputFileDescriptor(handle);
}

int RuntimeResourceManager::GetThreadCount(){
    return pImpl->GetThreadCount();
}

std::vector<int> RuntimeResourceManager::GetThreadIds(){
    return pImpl->GetThreadIds();
}

void RuntimeResourceManager::Kill(CrookedEditor::API::SubprocessHandle handle){
    pImpl->Kill(handle);
}

void RuntimeResourceManager::KillThread(CrookedEditor::API::ThreadHandle handle){
    pImpl->KillThread(handle);
}

bool RuntimeResourceManager::DoesThreadExist(CrookedEditor::API::ThreadHandle handle){
    return pImpl->DoesThreadExist(handle);
};

CrookedEditor::API::MemoryUsageData RuntimeResourceManager::MemoryUsage(){
    return pImpl->MemoryUsage();
};

void RuntimeResourceManager::Quit(){
    pImpl->Quit();
}
