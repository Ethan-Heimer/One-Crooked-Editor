#pragma once

#include "isystem.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

#include <memory>

namespace CrookedEditor::Application{
    class TextEditor final : public SystemHandling::ISystem{
        public:
            TextEditor(SystemHandling::SystemID SID, Events::RequestBus& eventBus); 

            ~TextEditor();

            SystemHandling::SystemStartOptions Start();
            bool Update();
            void Stop(SystemHandling::SystemTerminateReasons reason);

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
