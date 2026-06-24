#pragma once

#include "buffer.h"
#include <string>

using namespace std;

namespace Editor::Buffers {
    class IBufferFileHandler{
        public:
            IBufferFileHandler(const string fileName){}

            virtual std::shared_ptr<Buffers::Buffer> LoadNewBuffer() const = 0;
            virtual void WriteBufferToFile(const std::shared_ptr<Buffers::Buffer>& buffer) = 0;
    };
}
