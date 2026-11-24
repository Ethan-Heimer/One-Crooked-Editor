#pragma once

#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"
#include "isystem.hpp"

namespace CrookedEditor::Systems{
    struct InputCaptured : public Events::IEvent<int>{};

    class InputHandler final : public API::ISystem{
        public:
            using API::ISystem::ISystem;

            API::SystemStartOptions Start();
            bool Update();
            void Stop(API::SystemTerminateReasons reason);
    };
}
