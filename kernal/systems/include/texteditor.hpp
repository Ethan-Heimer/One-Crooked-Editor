#pragma once

#include "isystem.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

#include <memory>

namespace CrookedEditor::Systems{
    class TextEditor final : public API::ISystem{
        public:
            TextEditor(API::SystemID SID, Events::RequestBus& eventBus); 

            ~TextEditor();

            API::SystemStartOptions Start();
            bool Update();
            void Stop(API::SystemTerminateReasons reason);

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
