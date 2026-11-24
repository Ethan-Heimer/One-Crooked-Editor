#pragma once

#include "requestbus.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

namespace CrookedEditor::API{
    using SystemID = size_t;
    class ISystem{
        public:
            ISystem(SystemID SID, Events::RequestBus& requestBus) 
                : SID(SID), requestBus(requestBus){};

            virtual ~ISystem(){}

            virtual SystemStartOptions Start() = 0;
            virtual bool Update() = 0;
            virtual void Stop(SystemTerminateReasons reason) = 0;

        protected:
            SystemID SID;
            Events::RequestBus& requestBus;
    };
}
