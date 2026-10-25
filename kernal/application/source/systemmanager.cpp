#include "isystem.hpp"
#include "systemmamager.hpp"
#include <functional>
#include <memory>
#include <map>
#include <mutex>
#include <vector>
#include "application.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

using namespace CrookedEditor::Application;

struct SystemManager::Impl{
    struct SystemExecInfo{
        std::shared_ptr<ISystem> system;
        SystemStartOptions opts;
        ThreadHandle threadHandle = Application::Application::EmpytHandle;
    };

    class Application& application;
    SystemID nextSID = 0;

    std::mutex systemsLock;
    std::map<SystemID, SystemExecInfo> systems;

    Impl(class Application& application) : application(application){}

    void ExecuteSystemQueue(){
        std::vector<SystemID> removeQueue;

        ForEverySystem([&](SystemID SID, SystemExecInfo& info){
            bool result = info.system->Update();

            if(result)
                removeQueue.push_back(SID);
        });

        for(auto SID : removeQueue){
            StopSystem(SID, SystemTerminateReasons::ExecutionEnded);
        }
    }

    SystemID StartSystem(std::shared_ptr<ISystem> system){
        SystemID SID = nextSID;
        system->SID = SID;

        SystemStartOptions opts = system->Start();
        ThreadHandle threadHandle = Application::Application::EmpytHandle;

        if(opts.SeperateThread){
            threadHandle = CreateSystemThread(SID, system);
        }

        AddToSystems(SID, system, opts, threadHandle);

        nextSID++;
        return SID;
    };

    void StopSystem(SystemID SID, SystemTerminateReasons reason){ 
        if(!DoesSystemExist(SID))
            return;

        SystemExecInfo& info = systems[SID];
        if(info.threadHandle != Application::Application::EmpytHandle)
            application.KillThread(info.threadHandle);

        RemoveFromSystems(SID, reason);
    } 

    void StopAll(){
        ForEverySystem([&](SystemID SID, SystemExecInfo& info){
            info.system->Stop(SystemTerminateReasons::ShutDown);
        });

        systems={};
    }

    void RemoveFromSystems(SystemID SID, SystemTerminateReasons reason){
        if(!DoesSystemExist(SID))
            return;

        std::lock_guard<std::mutex> lock{systemsLock};
        systems[SID].system->Stop(reason);
        systems.erase(SID);
    }

    void AddToSystems(SystemID SID, std::shared_ptr<ISystem>& system, 
            SystemStartOptions opts, ThreadHandle threadHandle){
        std::lock_guard<std::mutex> lock{systemsLock};
        systems.emplace(SID, SystemExecInfo{system, opts, threadHandle});
    }

    bool DoesSystemExist(SystemID SID){
        std::lock_guard<std::mutex> lock{systemsLock};
        return systems.contains(SID);
    }

    void ForEverySystem(std::function<void(SystemID SID, SystemExecInfo& info)> callback){
        std::lock_guard<std::mutex> lock{systemsLock};
        for(auto& [SID, info] : systems){
            callback(SID, info);
        };
    }

    ThreadHandle CreateSystemThread(SystemID SID, std::shared_ptr<ISystem>& system){
        return application.SpawnThread([this, SID, system](const std::atomic<bool>& quit){
            while(!quit){
                bool result = system->Update();

                if(result){
                    RemoveFromSystems(SID, SystemTerminateReasons::ExecutionEnded);
                }
            }

        });
    }
};

SystemManager::SystemManager(class Application& application) 
    : pImpl(std::make_unique<Impl>(application)){}

SystemManager::~SystemManager(){};

SystemID SystemManager::StartSystem(std::shared_ptr<ISystem> system){
    return pImpl->StartSystem(std::move(system));
}

void SystemManager::StopSystem(SystemID SID){
    pImpl->StopSystem(SID, SystemTerminateReasons::Requested);
}
    
void SystemManager::ExecuteSystemQueue(){
    pImpl->ExecuteSystemQueue();
}

void SystemManager::StopAll(){
    pImpl->StopAll();
}
