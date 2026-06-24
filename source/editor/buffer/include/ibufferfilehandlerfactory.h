#pragma once

#include "ibufferfilehandler.h"
#include <memory>
namespace Editor::Buffers {
    class IBufferFileHandlerFactory{
        public:
            virtual std::shared_ptr<IBufferFileHandler> Instanciate(const string filename) = 0;
    };
}
