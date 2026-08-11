#include "inputmanager.h"

using namespace Systems::Input;

InputManager::InputManager(){
    tcgetattr(STDIN_FILENO, &originalState);

    struct termios raw = originalState;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

InputManager::~InputManager(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalState);
}

int InputManager::GetKeyInput() noexcept{ 
    int ch{};
    read(STDIN_FILENO, &ch, 1);
    key = ch;

    return ch;
}
