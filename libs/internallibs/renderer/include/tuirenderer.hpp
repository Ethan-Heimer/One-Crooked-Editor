#pragma once

#include "rendercommandtype.hpp"
#include "terminalinterface.hpp"

#include <memory>

namespace Rendering{
    class TUIRenderer{
        public:
            TUIRenderer();
            ~TUIRenderer();

            TUIRenderer(const TUIRenderer& other) = delete;
            TUIRenderer(TUIRenderer&& other);

            TUIRenderer& operator=(const TUIRenderer& other) = delete;
            TUIRenderer& operator=(TUIRenderer&& other);

            void DoCommand(Commands::RenderCommand command);

            void Start(TerminalInterface& interface);
            void Stop(TerminalInterface& interface);
            void Display(TerminalInterface& interface);

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
