#include "inputmanager.h"

#include <ncurses.h>

using namespace Systems::Input;

int InputManager::GetKeyInput() noexcept{
    return getch();
}
