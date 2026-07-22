#pragma once

#include "editorfilehandling/ieditorfilehandler.h"
#include <memory>

namespace Editor::FileHandling {
    class IFileHandlerFactory{
        public:
            virtual std::shared_ptr<IFileHandler> Instanciate(const std::string fileName) = 0;
    };
}
