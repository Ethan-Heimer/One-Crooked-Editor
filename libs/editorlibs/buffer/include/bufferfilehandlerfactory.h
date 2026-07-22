#pragma once

#include "editorfilehandling/ieditorfilehandler.h"
#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "ibufferfilehandler.h"
#include <memory>
#include <type_traits>

namespace Buffers {
    template<typename T>
    requires std::is_base_of_v<IBufferFileHandler, T>
    class BufferFileHandlerFactory : public Editor::FileHandling::IFileHandlerFactory{
        public:
            std::shared_ptr<IFileHandler> Instanciate(const string fileName) override{
                return std::make_shared<T>(fileName);
            };
    };
}
