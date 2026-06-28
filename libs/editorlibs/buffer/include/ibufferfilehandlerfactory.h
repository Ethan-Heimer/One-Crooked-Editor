#pragma once

#include <memory>
#include "ibufferfilehandler.h"

namespace Editor::Buffers {
    class IBufferFileHandlerFactory{
        public:
            virtual std::shared_ptr<IBufferFileHandler> Instanciate(const string filename) = 0;
    };
}
