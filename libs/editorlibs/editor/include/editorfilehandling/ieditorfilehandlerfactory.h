#pragma once

#include "editorfilehandling/ieditorfile.h"
#include "editorfilehandling/ieditorfilehandler.h"
#include <memory>
#include <type_traits>

namespace Editor::Files {
    template<typename T>
    requires std::is_base_of_v<IEditorFile, T>
    class IEditorFileHandlerFactory{
        public:
            virtual std::shared_ptr<IEditorFileHandler<T>> Instanciate(const string fileName) = 0;
    };
}
