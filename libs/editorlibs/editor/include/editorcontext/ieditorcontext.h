#pragma once

#include <memory>
#include <queue>
#include <type_traits>

#include "editorcommands/ieditorcommandmanager.h"
#include "editorcommands/ieditorcommandmanagerfactory.h"
#include "editorfilehandling/ieditorfilehandler.h"
#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "ieditable.h"
#include "ieditorstatecontextfactory.h"
#include "ieditorundohanderfactory.h"
#include "ieditorundohandler.h"

using namespace std;
using namespace Editor::Files;
using namespace Editor::Commands;
using namespace Editor::States;

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
            shared_ptr<IStateContext> stateContext; 
            shared_ptr<IEditorCommandManager> commandManager;
            shared_ptr<IEditorCommandUndoHandler> undoHandler;

            virtual void Initialize( 
                    shared_ptr<IEditorFileHandlerFactory<T>> fileHandlerFactory,
                    shared_ptr<IStateContextFactory> stateContextFactory, 
                    shared_ptr<IEditorCommandManagerFactory> commandManager,
                    shared_ptr<IEditorUndoHandlerFactory> undoHandlerFactory,
                    queue<int>* inputQueue, string fileName) = 0;

            virtual void Update() = 0;
    };
}
