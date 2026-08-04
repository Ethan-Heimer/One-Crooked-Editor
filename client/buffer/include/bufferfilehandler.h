#pragma once

#include "ieditable.h"

namespace CrookedEditor::Buffers {
    class BufferFileInterpreter{
        public:
            std::shared_ptr<Editor::IEditable> LoadFromFile(std::string_view fileName);
            void SaveToFile(std::string_view fileName, const Editor::IEditable& fileSaver);
    };
}
