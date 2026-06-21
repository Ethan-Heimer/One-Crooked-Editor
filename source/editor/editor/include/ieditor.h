#pragma once

#include <memory>

#include "buffer.h"
#include "inputmanager.h"

using namespace std;

namespace Editor{
    class IEditor{
        public:
            bool quit;

            shared_ptr<Buffers::Buffer> buffer;
            weak_ptr<Systems::Input::IInputManager> inputManager;

            virtual void Save() = 0;

        protected:
            string fileName;

    };
}
