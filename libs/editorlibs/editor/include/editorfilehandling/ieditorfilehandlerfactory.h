#pragma once

#include "editorfilehandling/ieditorfile.h"
#include "editorfilehandling/ieditorfilehandler.h"
#include <memory>
#include <type_traits>

namespace Editor::Files {
    template<typename T>
    requires std::is_base_of_v<IFile, T>
    class IFileHandlerFactory{
        public:
            virtual std::shared_ptr<IFileHandler<T>> Instanciate(const string fileName) = 0;
    };
}
