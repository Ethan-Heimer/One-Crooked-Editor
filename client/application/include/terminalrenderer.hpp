#pragma once

#include "application.hpp"
#include "rendercommandtype.hpp"
#include "terminal.hpp"
#include <memory>
namespace CrookedEditor::Application{
    class TerminalRenderer{
        public:
            TerminalRenderer(Application& application, Terminal::TerminalController& terminalController);
            ~TerminalRenderer();

            void Start();
            void Stop();

            void AddCommand(Rendering::Commands::RenderCommand command);

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
