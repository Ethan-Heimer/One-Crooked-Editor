#pragma once

#include "terminal.hpp"
#include <termios.h>
#include <unistd.h>

namespace Systems::Input{
    class InputManager{
        public:
            InputManager(Terminal::TerminalController& terminalController);
            int GetKeyInput() noexcept;

        private:
            int key;
    };
}
