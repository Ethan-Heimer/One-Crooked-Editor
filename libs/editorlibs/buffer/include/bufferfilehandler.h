#pragma once

#include "ibufferfilehandler.h"

namespace Editor::Buffers {
    class BufferFileHandler : public IBufferFileHandler{
        public:
            const string fileName;
            BufferFileHandler(const string fileName);

            std::shared_ptr<Buffers::Buffer> LoadFromFile() override;
            void SaveToFile(const std::shared_ptr<IFileSaveable> fileSaver) override;
    };
}
