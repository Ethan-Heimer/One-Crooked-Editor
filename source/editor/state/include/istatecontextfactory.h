#pragma once

#include "buffer.h"
#include "ibufferfilehandler.h"
#include "icontext.h"
#include "iinputmanager.h"

namespace Editor::States{
    class IStateContextFactory{
        public:
            virtual std::shared_ptr<IStateContext> Instanciate(std::weak_ptr<Buffers::IBufferFileHandler> fileHandler,
                    std::weak_ptr<Buffers::Buffer> buffer, weak_ptr<Systems::Input::IInputManager> inputManager, bool* quitToken) = 0;
    };
}
