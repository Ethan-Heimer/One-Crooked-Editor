#pragma once

#include <memory>
#include <iinputmanager.h>
#include "ibufferfilehandler.h"

namespace Editor::States{
    enum States{
        Normal = 0,
        Insert = 1
    };

    class IStateContextPasskey{
        protected:
            IStateContextPasskey(){};
    };

    class IStateContext{
        public:
        weak_ptr<Systems::Input::IInputManager> inputManager;
        weak_ptr<Buffers::IBufferFileHandler> fileHandler;
        weak_ptr<Buffers::Buffer> buffer;
        bool* quitToken;

        IStateContext(IStateContextPasskey passkey){};

        virtual void Initialize(weak_ptr<Buffers::IBufferFileHandler> fileHandler, 
                    weak_ptr<Buffers::Buffer> buffer, weak_ptr<Systems::Input::IInputManager> inputManager, bool* quitToken) = 0;
        virtual void Update() = 0;
        virtual void ChangeState(States state) = 0;
    };
}
