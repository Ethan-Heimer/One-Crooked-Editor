#pragma once

#include "editorfilehandling/ieditorfile.h"
#include "editorfilehandling/ieditorfilehandler.h"
#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "ibufferfilehandler.h"
#include <memory>
#include <type_traits>

namespace Buffers {
    template<typename E, typename T>
    requires std::is_base_of_v<IFile, E>
    && std::is_base_of_v<IBufferFileHandler, T>
    class BufferFileHandlerFactory : public Editor::Files::IFileHandlerFactory<E>{
        public:
            virtual std::shared_ptr<IFileHandler<E>> Instanciate(const string fileName){
                return std::make_shared<T>(fileName);
            };
    };
}
