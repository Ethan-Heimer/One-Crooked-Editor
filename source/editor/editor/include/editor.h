#pragma once

#include "buffer.h"

using namespace std;
using namespace Editor::Buffers;

namespace Editor{
    class Editor{
        public:
            shared_ptr<Buffer> buffer;
            const string fileName;

            bool quit;

            Editor(const string fileName);

            void Save();
    };
}
