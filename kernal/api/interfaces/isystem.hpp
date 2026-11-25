#pragma once

#include "requestbus.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"
#include "systemtypes.hpp"

namespace CrookedEditor::API{
    class ISystem{
        public:
            ISystem() : SID(NoSID), requestBus(nullptr){}
            ISystem(SystemID SID, Events::RequestBus* requestBus) 
                : SID(SID), requestBus(requestBus){};

            virtual ~ISystem(){}

            virtual SystemStartOptions Start() = 0;
            virtual bool Update() = 0;
            virtual void Stop(SystemTerminateReasons reason) = 0;

            virtual std::shared_ptr<ISystem> Instanciate(SystemID SID, 
                    Events::RequestBus* requestBus) const = 0;

            bool IsExecutable() const {
                return SID != NoSID || requestBus != nullptr;
            }

        protected:
            SystemID SID;
            Events::RequestBus* requestBus;
    };
}
