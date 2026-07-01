#pragma once

#include <memory>
#include <queue>
#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "ieditorcontext.h"

using namespace std;

namespace Editor{
    template<typename T>
    requires std::is_base_of_v<IEditable, T> &&
    std::is_base_of_v<IEditorFile, T>
    class EditorContext : public IEditorContext<T>{
        public:
            using IEditorContext<T>::IEditorContext;

            virtual void Initialize( 
                    shared_ptr<IEditorFileHandlerFactory<T>> fileHandlerFactory,
                    shared_ptr<States::IStateContextFactory> stateContextFactory,
                    shared_ptr<IEditorCommandManagerFactory> commandManagerFactory,
                    queue<int>* inputQueue, string fileName) override{

                this->fileHandler = fileHandlerFactory->Instanciate(fileName);
                this->buffer = this->fileHandler->LoadFromFile();
                this->commandManager = commandManagerFactory->Instanciate(this->buffer);

                this->stateContext = stateContextFactory->Instanciate
                    (this->fileHandler, this->buffer, this->commandManager, inputQueue, &(this->quit));
            }

            void Update() override{
                this->stateContext->Update();
            }
    };
}

