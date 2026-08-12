#pragma once

#include "rendercommand.hpp"
#include "terminal.hpp"

#include <memory>

namespace Rendering{
    class TUIRenderer{
        public:
            TUIRenderer(Terminal::TerminalController& terminalController);
            ~TUIRenderer();

            TUIRenderer(const TUIRenderer& other) = delete;
            TUIRenderer(TUIRenderer&& other) = default;

            void DoCommand(Commands::RenderCommand& command);
            void Display();

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
