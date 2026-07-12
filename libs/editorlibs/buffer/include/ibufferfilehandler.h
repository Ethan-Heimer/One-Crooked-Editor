#pragma once

#include <string>

#include "buffer.h"
#include "editorfilehandling/ieditorfilehandler.h"

using namespace std;
using namespace FileHandling;
using namespace Editor::Files;

namespace Buffers {
    class IBufferFileHandler : public IFileHandler<Buffer>{
        public:
            IBufferFileHandler(const string fileName){}
    };
}
