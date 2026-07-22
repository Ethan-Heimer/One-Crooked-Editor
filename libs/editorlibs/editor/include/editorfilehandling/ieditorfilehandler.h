#pragma once

#include "ieditable.h"
#include <memory>

namespace Editor::FileHandling {
    class IFileHandler{
        public:
            virtual std::shared_ptr<Editor::IEditable> LoadFromFile() = 0;
            virtual void SaveToFile(const std::shared_ptr<IEditable> fileSaver)  = 0;
    };
}
