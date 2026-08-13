#include "inputmanager.h"
#include "terminal.hpp"

using namespace Systems::Input;

InputManager::InputManager(Terminal::TerminalController& terminalController){
    terminalController.EnterRawMode();
}

int InputManager::GetKeyInput() noexcept{ 
    int ch{};
    read(STDIN_FILENO, &ch, 1);
    key = ch;

    return ch;
}
