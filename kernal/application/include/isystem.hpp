#pragma once

#include <cstddef>
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

namespace CrookedEditor::Application{
    using SystemID = size_t;

    class ISystem{
        public:
            SystemID SID;

            virtual SystemStartOptions Start() = 0;
            virtual bool Update() = 0;
            virtual void Stop(SystemTerminateReasons reason) = 0;

            virtual ~ISystem(){}
    };
}
