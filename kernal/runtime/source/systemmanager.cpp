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
#include "runtimeresourcemanager.hpp"
#include "systemconstructor.hpp"
#include "systemterminatereasons.hpp"

using namespace CrookedEditor::Runtime;
using namespace Events;

struct SystemManager::Impl{
    struct SystemExecInfo{
        std::shared_ptr<API::ISystem> system;
        API::SystemStartOptions opts;
        API::ThreadHandle threadHandle = API::EmptyHandle;
    };

    std::mutex systemsLock;

    RuntimeResourceManager& resourceManager;
    RequestBus& requestBus;

    API::SystemID nextSID = 0;

    std::map<API::SystemID, SystemExecInfo> systems;
    std::vector<API::SystemID> removeQueue;

    Impl(RuntimeResourceManager& resourceManager, RequestBus& requestBus) 
        : resourceManager(resourceManager), requestBus(requestBus){}

    void ExecuteSystemQueue(){
        ForEverySystem([&](API::SystemID SID, const SystemExecInfo& info){
            if(info.opts.SkipUpdateLoop)
                return;

            if(info.threadHandle != API::EmptyHandle)
                return;

            bool result = info.system->Update();
            if(result)
                removeQueue.push_back(SID);
        });

        for(auto SID : removeQueue){
            StopSystem(SID, API::SystemTerminateReasons::ExecutionEnded);
        }
    }

    API::SystemID StartSystem(SystemConstructor&& systemConstructor){
        API::SystemID SID = nextSID;
        std::shared_ptr<API::ISystem> system = systemConstructor.ConstructSystem(SID, requestBus);

        API::SystemStartOptions opts = system->Start();
        API::ThreadHandle threadHandle = API::EmptyHandle;

        if(opts.SeperateThread && !opts.SkipUpdateLoop)
            threadHandle = CreateSystemThread(SID, system);

        AddToSystems(SID, system, opts, threadHandle);

        nextSID++;
        return SID;
    };

    void StopSystem(API::SystemID SID, API::SystemTerminateReasons reason){ 
        if(!DoesSystemExist(SID))
            return;

        SystemExecInfo& info = systems[SID];
        if(info.threadHandle != API::EmptyHandle)
            resourceManager.KillThread(info.threadHandle);

        RemoveFromSystems(SID, reason);
    } 

    void StopAll(){
        ForEverySystem([&](API::SystemID SID, const SystemExecInfo& info){
            if(info.threadHandle != API::EmptyHandle)
                resourceManager.KillThread(info.threadHandle);

            info.system->Stop(API::SystemTerminateReasons::ShutDown);
        });

        systems={};
    }

    void RemoveFromSystems(API::SystemID SID, API::SystemTerminateReasons reason){
        if(!DoesSystemExist(SID))
            return;

        std::lock_guard<std::mutex> lock{systemsLock};
        systems[SID].system->Stop(reason);
        systems.erase(SID);
    }

    void AddToSystems(API::SystemID SID, std::shared_ptr<API::ISystem>& system, 
            API::SystemStartOptions opts, API::ThreadHandle threadHandle){
        std::lock_guard<std::mutex> lock{systemsLock};
        systems.emplace(SID, SystemExecInfo{system, opts, threadHandle});
    }

    bool DoesSystemExist(API::SystemID SID){
        std::lock_guard<std::mutex> lock{systemsLock};
        return systems.contains(SID);
    }

    void ForEverySystem(std::function<void(API::SystemID SID, const SystemExecInfo& info)> callback){
        std::lock_guard<std::mutex> lock{systemsLock};
        for(const auto& [SID, info] : systems){
            callback(SID, info);
        };
    }

    API::ThreadHandle CreateSystemThread(API::SystemID SID, std::shared_ptr<API::ISystem>& system){
        return resourceManager.SpawnThread([this, SID, system](const std::atomic<bool>& quit){
            while(!quit){
                bool result = system->Update();

                if(result){
                    RemoveFromSystems(SID, API::SystemTerminateReasons::ExecutionEnded);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

        });
    }
};

SystemManager::SystemManager(RuntimeResourceManager& resourceManager, RequestBus& runtimeRequestBus) 
    : pImpl(std::make_unique<Impl>(resourceManager, runtimeRequestBus)){}

SystemManager::~SystemManager() = default;

CrookedEditor::API::SystemID SystemManager::StartSystemP(SystemConstructor&& systemConstructor){
    return pImpl->StartSystem(std::move(systemConstructor));
}

void SystemManager::StopSystem(API::SystemID SID){
    pImpl->StopSystem(SID, API::SystemTerminateReasons::Requested);
}
    
void SystemManager::ExecuteSystemQueue(){
    pImpl->ExecuteSystemQueue();
}

void SystemManager::StopAll(){
    pImpl->StopAll();
}
