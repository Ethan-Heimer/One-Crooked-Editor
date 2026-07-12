#pragma once

#include <algorithm>
#include <memory>
#include <queue>
#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "ieditorcontext.h"
#include "ieditorundohanderfactory.h"

using namespace std;
using namespace Editor::States;

namespace Editor{
    template<typename T>
    requires std::is_base_of_v<IEditable, T> &&
    std::is_base_of_v<IEditorFile, T>
    class EditorContext : public IEditorContext<T>{
        public:
            using IEditorContext<T>::IEditorContext;
            void Initialize( 
                    shared_ptr<IEditorFileHandlerFactory<T>> fileHandlerFactory,
                    shared_ptr<IStateContextFactory> stateContextFactory, 
                    shared_ptr<IEditorCommandManagerFactory> commandManagerFactory,
                    shared_ptr<IEditorUndoHandlerFactory> undoHandlerFactory,
                    queue<int>* inputQueue, string fileName) override {

                this->fileHandler = fileHandlerFactory->Instanciate(fileName);
                this->buffer = this->fileHandler->LoadFromFile();
                this->undoHandler = undoHandlerFactory->Instanciate();
                this->commandManager = commandManagerFactory->Instanciate(this->buffer, this->undoHandler);

                this->stateContext = stateContextFactory->Instanciate
                    (this->fileHandler, this->buffer, this->commandManager, this->undoHandler, inputQueue, &(this->quit));
            }

            void Update() override{
                this->stateContext->Update();
            }
    };
}

