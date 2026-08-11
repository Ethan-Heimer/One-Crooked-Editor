#pragma once

#include "iinputmanager.h"

#include <termios.h>
#include <unistd.h>

namespace Systems::Input{
    class InputManager : public IInputManager{
        public:
            InputManager();
            ~InputManager() override;

            int GetKeyInput() noexcept override;

        private:
            struct termios originalState;
            int key;
    };
}
