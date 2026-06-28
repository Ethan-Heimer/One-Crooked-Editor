#pragma once

#include <string>

#include "buffer.h"
#include "filehandling.h"

using namespace std;
using namespace FileHandling;

namespace Editor::Buffers {
    class IBufferFileHandler : public IFileSaver, public IFileLoader<Buffers::Buffer>{
        public:
            IBufferFileHandler(const string fileName){}
    };
}
