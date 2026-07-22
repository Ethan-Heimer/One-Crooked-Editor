#pragma once

#include <string>

#include "editorfilehandling/ieditorfilehandler.h"

using namespace std;
using namespace Editor::FileHandling;

namespace Buffers {
    class IBufferFileHandler : public IFileHandler{
        public:
            IBufferFileHandler(const string fileName){}
    };
}
