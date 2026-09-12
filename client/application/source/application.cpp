#include "application.hpp"
#include "thread"
#include <chrono>
#include <cstddef>
#include <map>
#include <memory>
#include <thread>
#include <unistd.h>
#include "subprocess.hpp"

using namespace CrookedEditor::Application;
using namespace Process;

struct Application::Impl{
    public: 
        ~Impl(){
            for(auto& pair : threadQuitTokens){
                threadQuitTokens[pair.first] = true;
            }

            for(auto& pair : threads){
                pair.second.join(); 
            }

            for(auto& pair : subprocesses){
                pair.second.Kill();
            }
        }

        ThreadHandle SpawnThread(const ThreadConstructor& threadConstructor){
            threadQuitTokens[lastThreadID] = false;
            threads[lastThreadID] = threadConstructor.CreateThread(threadQuitTokens.at(lastThreadID)); 
            lastThreadID++;

            return lastThreadID;
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
            threadQuitTokens[handle] = true;
            threads[handle].join();

            threads.erase(handle);
            threadQuitTokens.erase(handle);
        }


        void Run(){
            while(!quitApplication){
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }

        void Quit(){
            quitApplication = true;
        }

    private:
        std::map<int, std::atomic<bool>> threadQuitTokens;
        std::map<int, std::thread> threads;

        std::map<int, Subprocess> subprocesses;

        int lastThreadID;
        int lastSubprocessID;

        std::atomic<bool> quitApplication{};

};

Application::Application() : pImpl(std::make_unique<Impl>()){}
Application::~Application(){
    pImpl.reset();
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

void Application::Kill(SubprocessHandle handle){
    pImpl->Kill(handle);
}

void Application::KillThread(ThreadHandle handle){
    pImpl->KillThread(handle);
}

void Application::Run(){
    pImpl->Run();
}

void Application::Quit(){
    pImpl->Quit();
}

