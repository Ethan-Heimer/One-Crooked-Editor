#pragma once

#include "ibufferfilehandler.h"

namespace Editor::Buffers {
    class BufferFileHandler : public IBufferFileHandler{
        public:
            const string fileName;
            BufferFileHandler(const string fileName);

            std::shared_ptr<Buffers::Buffer> LoadNewBuffer() const override;
            void WriteBufferToFile(const std::shared_ptr<Buffers::Buffer>& buffer) override;
    };
}
