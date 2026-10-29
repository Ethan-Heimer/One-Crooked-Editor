#pragma once

#include "isystem.hpp"
#include "systemterminatereasons.hpp"

namespace CrookedEditor::System{
    class FooSystem final : public Application::SystemHandling::ISystem{
        public:
            using Application::SystemHandling::ISystem::ISystem;

            Application::SystemHandling::SystemStartOptions Start() override;
            bool Update() override;
            void Stop(Application::SystemHandling::SystemTerminateReasons reason) override;
    };
}
