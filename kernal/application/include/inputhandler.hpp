#pragma once

#include <memory>
#include "application.hpp"
#include "terminal.hpp"

namespace CrookedEditor::Application{
    class InputHandler{
        public:
            InputHandler(Application& application, Terminal::TerminalController& terminalController);
            ~InputHandler();

            void Start();
            void Stop();

            int GetInput();

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
