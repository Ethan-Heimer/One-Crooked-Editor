#pragma once

#include <memory>
#include <queue>

#include "buffer.h"
#include "ibufferfilehandlerfactory.h"
#include "ieditorstatecontextfactory.h"

using namespace std;

namespace Editor{
    class IEditorContextPasskey{
        protected:
            IEditorContextPasskey(){};
    };

    class IEditorContext{
        public:
            IEditorContext(IEditorContextPasskey passkey){};

            bool quit;

            shared_ptr<Buffers::IBufferFileHandler> bufferFileHandler;
            shared_ptr<Buffers::Buffer> buffer;
            shared_ptr<States::IStateContext> stateContext; 

            virtual void Initialize( 
                    shared_ptr<Buffers::IBufferFileHandlerFactory> bufferFileHandlerFactory,
                    shared_ptr<States::IStateContextFactory> stateContextFactory, queue<int>* inputQueue, string fileName) = 0;

            virtual void Update() = 0;
    };
}
