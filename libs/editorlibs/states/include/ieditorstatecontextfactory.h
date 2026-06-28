#pragma once

#include "buffer.h"
#include "ibufferfilehandler.h"
#include "ieditorstatecontext.h"
#include <queue>

namespace Editor::States{
    class IStateContextFactory{
        public:
            virtual std::shared_ptr<IStateContext> Instanciate(std::weak_ptr<Buffers::IBufferFileHandler> fileHandler,
                    std::weak_ptr<Buffers::Buffer> buffer, queue<int>* inputQueue, bool* quitToken) = 0;
    };
}
