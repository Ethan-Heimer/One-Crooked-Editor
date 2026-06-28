#pragma once

#include <memory>
#include <queue>
#include <type_traits>

#include "editorfilehandling/ieditorfilehandler.h"
#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "ieditable.h"
#include "ieditorstatecontextfactory.h"

using namespace std;
using namespace Editor::Files;

namespace Editor{
    class IEditorContextPasskey{
        protected:
            IEditorContextPasskey(){};
    };

    template<typename T>
    requires std::is_base_of_v<IEditable, T>
    class IEditorContext{
        public:
            IEditorContext(IEditorContextPasskey passkey){};

            bool quit;

            shared_ptr<IEditorFileHandler<T>> fileHandler;
            shared_ptr<T> buffer;
            shared_ptr<States::IStateContext> stateContext; 

            virtual void Initialize( 
                    shared_ptr<IEditorFileHandlerFactory<T>> fileHandlerFactory,
                    shared_ptr<States::IStateContextFactory> stateContextFactory, queue<int>* inputQueue, string fileName) = 0;

            virtual void Update() = 0;
    };
}
