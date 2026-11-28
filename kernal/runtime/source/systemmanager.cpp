#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <map>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>
#include <ranges>

#include "requestbus.hpp"
#include "runtimeevents.hpp"
#include "runtimeresourcemanager.hpp"
#include "systemterminatereasons.hpp"
#include "systemtypes.hpp"

#include "applicationtypes.hpp"
#include "isystem.hpp"
#include "systemmanager.hpp"

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

    API::SystemID nextSID = API::NoSID+1;

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

            if(!info.system->IsExecutable())
                return;

            bool result = ExecuteUpdate(info.system);
            if(result)
                removeQueue.push_back(SID);
        });

        for(auto SID : removeQueue){
            StopSystem(SID, API::SystemTerminateReasons::ExecutionEnded);
        }
    }

    API::SystemID StartSystem(const API::ISystem& system){
        API::SystemID SID = nextSID;
        std::shared_ptr<API::ISystem> newSystem = system.Instanciate(nextSID, &requestBus);

        if(!newSystem->IsExecutable())
            return API::NoSID;

        API::ThreadHandle threadHandle = API::EmptyHandle;
        API::SystemStartOptions opts = newSystem->Start();

        if(opts.SeperateThread && !opts.SkipUpdateLoop)
            threadHandle = CreateSystemThread(SID, newSystem);

        AddToSystems(SID, newSystem, opts, threadHandle);

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
        auto keyView = std::views::keys(systems);
        std::vector<API::SystemID> ids{keyView.begin(), keyView.end()};

        int count = 0;
        for(auto& id : ids){
            StopSystem(id, API::SystemTerminateReasons::ShutDown);
        }

        systems = {};
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
        if(DoesSystemExist(SID) && SID != API::NoSID)
            return;

        std::lock_guard<std::mutex> lock{systemsLock};
        systems.emplace(SID, SystemExecInfo{system, opts, threadHandle});
    }

    bool DoesSystemExist(API::SystemID SID){
        if(SID == API::NoSID)
            return false;

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
                bool result = ExecuteUpdate(system);

                if(result){
                    RemoveFromSystems(SID, API::SystemTerminateReasons::ExecutionEnded);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

        });
    }

    bool ExecuteUpdate(const std::shared_ptr<API::ISystem>& system){
        try{
            return system->Update();
        } catch (const std::exception& e) {
            std::stringstream log;
            log << "An Error Occured and system of ID ";
            log << system->GetSID();
            log << " has been shutdown: ";
            log << e.what();

            std::ignore = requestBus.SendRequest<API::RuntimeLog>(log.str());
            return true; 
        }
    }
};

SystemManager::SystemManager(RuntimeResourceManager& resourceManager, RequestBus& runtimeRequestBus) 
    : pImpl(std::make_unique<Impl>(resourceManager, runtimeRequestBus)){}

SystemManager::~SystemManager() = default;

CrookedEditor::API::SystemID SystemManager::StartSystemP(const API::ISystem& system){
    return pImpl->StartSystem(system);
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
