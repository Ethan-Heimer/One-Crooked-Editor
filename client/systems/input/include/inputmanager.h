#pragma once

#include "iinputmanager.h"

namespace Systems::Input{
    class InputManager : public IInputManager{
        public:
            int GetKeyInput() noexcept override;

        private:
            int key;
    };
}
