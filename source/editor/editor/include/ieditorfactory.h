#pragma once

#include "ieditor.h"
#include "istatecontextfactory.h"

namespace Editor {
    class IEditorFactory{
        public:
            virtual std::shared_ptr<IEditor> 
                Instanciate(weak_ptr<Buffers::IBufferFileHandlerFactory> bufferFileHandlerFactory, weak_ptr<States::IStateContextFactory> stateContextFactory,
                        weak_ptr<Systems::Input::IInputManager> inputManager, string fileName) = 0;
    };
}
