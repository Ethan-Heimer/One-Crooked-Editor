#pragma once

#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"
#include "isystem.hpp"

namespace CrookedEditor::Application{
    struct InputCaptured : public Events::IEvent<int>{};

    class InputHandler final : public SystemHandling::ISystem{
        public:
            using SystemHandling::ISystem::ISystem;

            SystemHandling::SystemStartOptions Start();
            bool Update();
            void Stop(SystemHandling::SystemTerminateReasons reason);
    };
}
