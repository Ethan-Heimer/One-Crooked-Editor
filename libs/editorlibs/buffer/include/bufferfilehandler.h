#pragma once

#include "ibufferfilehandler.h"
#include "ieditable.h"

namespace Buffers {
    class BufferFileHandler : public IBufferFileHandler{
        public:
            const string fileName;
            BufferFileHandler(const string fileName);

            std::shared_ptr<Editor::IEditable> LoadFromFile() override;
            void SaveToFile(const std::shared_ptr<Editor::IEditable> fileSaver) override;
    };
}
