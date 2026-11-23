#pragma once

#include "isystem.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"
#include "rendercommandconstructor.hpp"

#include <memory>

namespace CrookedEditor::Application{
    struct PublishRenderCommand 
        : public Events::IEvent<std::shared_ptr<Rendering::Commands::IRenderCommandConstructor>>{};

    class TerminalRenderer final : public SystemHandling::ISystem{
        public:
            TerminalRenderer(SystemHandling::SystemID SID, Events::RequestBus& events);
            ~TerminalRenderer();

            SystemHandling::SystemStartOptions Start();
            bool Update();
            void Stop(SystemHandling::SystemTerminateReasons reason);

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
