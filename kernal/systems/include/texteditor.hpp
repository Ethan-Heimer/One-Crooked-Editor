#pragma once

#include "isystem.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

#include <memory>

namespace CrookedEditor::Systems{
    class TextEditor final : public API::ISystem{
        public:
            TextEditor();
            TextEditor(API::SystemID SID, Events::RequestBus* eventBus); 

            ~TextEditor();

            API::SystemStartOptions Start();
            bool Update();
            void Stop(API::SystemTerminateReasons reason);

            std::shared_ptr<API::ISystem> Instanciate(API::SystemID SID, 
                    Events::RequestBus* requestBus) const;

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
