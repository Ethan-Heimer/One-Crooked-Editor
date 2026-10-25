#pragma once

#include "isystem.hpp"
#include "systemterminatereasons.hpp"
#include <fstream>

namespace CrookedEditor::System{
    class FooSystem final : public Application::ISystem{
        public:
            Application::SystemStartOptions Start() override;
            bool Update() override;
            void Stop(Application::SystemTerminateReasons reason) override;

        private:
            std::ofstream output{};
    };
}
