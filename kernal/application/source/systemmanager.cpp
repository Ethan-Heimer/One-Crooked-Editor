#include "application.hpp"
#include "applicationtypes.hpp"
#include "isystem.hpp"
#include "systemmanager.hpp"
#include <functional>
#include <memory>
#include <map>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>
#include "requestbus.hpp"
#include "systemconstructor.hpp"
#include "systemterminatereasons.hpp"

using namespace CrookedEditor::Application;
using namespace CrookedEditor::Application::SystemHandling;
using namespace Events;

struct SystemManager::Impl{
    struct SystemExecInfo{
        std::shared_ptr<ISystem> system;
        SystemStartOptions opts;
        ThreadHandle threadHandle = EmptyHandle;
    };

    std::mutex systemsLock;

    Application& application;
    RequestBus& requestBus;

    SystemID nextSID = 0;

    std::map<SystemID, SystemExecInfo> systems;
    std::vector<SystemID> removeQueue;

    Impl(Application& application, RequestBus& requestBus) 
        : application(application), requestBus(requestBus){}

    void ExecuteSystemQueue(){
        ForEverySystem([&](SystemID SID, const SystemExecInfo& info){
            if(info.opts.SkipUpdateLoop)
                return;

            if(info.threadHandle != EmptyHandle)
                return;

            bool result = info.system->Update();
            if(result)
                removeQueue.push_back(SID);
        });

        for(auto SID : removeQueue){
            StopSystem(SID, SystemTerminateReasons::ExecutionEnded);
        }
    }

    SystemID StartSystem(SystemConstructor&& systemConstructor){
        SystemID SID = nextSID;
        std::shared_ptr<ISystem> system = systemConstructor.ConstructSystem(SID, requestBus);

        SystemStartOptions opts = system->Start();
        ThreadHandle threadHandle = EmptyHandle;

        if(opts.SeperateThread && !opts.SkipUpdateLoop)
            threadHandle = CreateSystemThread(SID, system);

        AddToSystems(SID, system, opts, threadHandle);

        nextSID++;
        return SID;
    };

    void StopSystem(SystemID SID, SystemTerminateReasons reason){ 
        if(!DoesSystemExist(SID))
            return;

        SystemExecInfo& info = systems[SID];
        if(info.threadHandle != EmptyHandle)
            application.KillThread(info.threadHandle);

        RemoveFromSystems(SID, reason);
    } 

    void StopAll(){
        ForEverySystem([&](SystemID SID, const SystemExecInfo& info){
            if(info.threadHandle != EmptyHandle)
                application.KillThread(info.threadHandle);

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

    void ForEverySystem(std::function<void(SystemID SID, const SystemExecInfo& info)> callback){
        std::lock_guard<std::mutex> lock{systemsLock};
        for(const auto& [SID, info] : systems){
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

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

        });
    }
};

SystemManager::SystemManager(Application& application, RequestBus& runtimeRequestBus) 
    : pImpl(std::make_unique<Impl>(application, runtimeRequestBus)){}

SystemManager::~SystemManager() = default;

SystemID SystemManager::StartSystemP(SystemConstructor&& systemConstructor){
    return pImpl->StartSystem(std::move(systemConstructor));
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
