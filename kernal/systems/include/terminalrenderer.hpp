#pragma once

#include "isystem.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"
#include "rendercommandconstructor.hpp"

#include <memory>

namespace CrookedEditor::Systems{
    struct PublishRenderCommand 
        : public Events::IEvent<std::shared_ptr<Rendering::Commands::IRenderCommandConstructor>>{};

    class TerminalRenderer final : public API::ISystem{
        public:
            TerminalRenderer(API::SystemID SID, Events::RequestBus& events);
            ~TerminalRenderer();

            API::SystemStartOptions Start();
            bool Update();
            void Stop(API::SystemTerminateReasons reason);

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
