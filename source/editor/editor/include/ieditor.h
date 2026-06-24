#pragma once

#include <memory>

#include "buffer.h"
#include "ibufferfilehandlerfactory.h"
#include "iinputmanager.h"
#include "istatecontextfactory.h"

using namespace std;

namespace Editor{
    class IEditorPasskey{
        protected:
            IEditorPasskey(){};
    };

    class IEditor{
        public:
            IEditor(IEditorPasskey passkey){};

            bool quit;

            shared_ptr<Buffers::IBufferFileHandler> bufferFileHandler;
            shared_ptr<Buffers::Buffer> buffer;
            shared_ptr<States::IStateContext> stateContext; 

            virtual void Initialize(weak_ptr<Systems::Input::IInputManager> inputManager, 
                    string fileName, weak_ptr<Buffers::IBufferFileHandlerFactory> bufferFileHandlerFactory,
                    weak_ptr<States::IStateContextFactory> stateContextFactory) = 0;

            virtual void Update() = 0;
    };
}
