#pragma once

#define ctrl(x) ((x) & 0x1f)

namespace Systems::Input{
    class IInputManager{
        public:
            virtual ~IInputManager(){}
            virtual int GetKeyInput() noexcept = 0;
    };
}
