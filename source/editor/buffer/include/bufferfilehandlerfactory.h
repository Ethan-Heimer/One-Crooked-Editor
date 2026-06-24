#pragma once

#include "ibufferfilehandler.h"
#include "ibufferfilehandlerfactory.h"
#include <memory>
#include <type_traits>

namespace Editor::Buffers {
    template <typename T>
    requires std::is_base_of<IBufferFileHandler, T>::value
    class BufferFileHandlerFactory : public IBufferFileHandlerFactory{  
        public:
            std::shared_ptr<IBufferFileHandler> Instanciate(const string filename) override{
                std::shared_ptr<IBufferFileHandler> fileHandler = std::make_shared<T>(filename);
                return std::move(fileHandler);
            }
    };
}
