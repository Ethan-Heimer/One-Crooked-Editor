#pragma once

#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "ieditorcommandmanagerfactory.h"
#include "ieditorcontext.h"
#include "ieditorstatecontextfactory.h"
#include "ieditorundohanderfactory.h"
#include <memory>

namespace Editor {
    template<typename T>
    requires std::is_base_of_v<IEditable, T>
    class IEditorContextFactory{
        public:
                IEditorContextFactory(
                std::shared_ptr<States::IStateContextFactory> stateContextFactory,
                std::shared_ptr<IEditorFileHandlerFactory<T>> bufferFileHandlerFactory,
                std::shared_ptr<IEditorCommandManagerFactory> commandManagerFactory,
                std::shared_ptr<IEditorUndoHandlerFactory> undoHandlerFactory)

                : stateContextFactory(stateContextFactory), 
                bufferFileHandlerFactory(bufferFileHandlerFactory), 
                commandManagerFactory(commandManagerFactory),
                undoHandlerFactory(undoHandlerFactory){};

            virtual std::shared_ptr<IEditorContext<T>> 
                Instanciate(queue<int>* inputQueue, string fileName) const = 0;

        protected:
            std::shared_ptr<States::IStateContextFactory> stateContextFactory;
            std::shared_ptr<IEditorFileHandlerFactory<T>> bufferFileHandlerFactory;
            std::shared_ptr<IEditorCommandManagerFactory> commandManagerFactory;
            std::shared_ptr<IEditorUndoHandlerFactory> undoHandlerFactory;
    };
}
