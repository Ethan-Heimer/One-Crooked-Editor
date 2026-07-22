#pragma once

#include <memory>
#include <queue>
#include <string>
#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "editorstatecontext.h"
#include "editorundohandler.h"
#include "ieditable.h"
#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "editorcommandmanager.h"

namespace Editor{
    class EditorContext{
        public:
            Commands::UndoHandler undoHandler{};
            Commands::CommandManager commandManager{undoHandler};
            States::StateContext stateContext{commandManager, undoHandler};

            std::shared_ptr<IEditable> buffer;

            std::shared_ptr<FileHandling::IFileHandler> fileHandler;
            bool quit;

            EditorContext( 
                    shared_ptr<FileHandling::IFileHandlerFactory> fileHandlerFactory,
                    queue<int>* inputQueue, string fileName){

                this->fileHandler = fileHandlerFactory->Instanciate(fileName);
                this->buffer = this->fileHandler->LoadFromFile();

                this->commandManager.Initialize(this->buffer);
                this->stateContext.Initialize(buffer, fileHandler, "Normal", inputQueue, &quit);
            }

            void Update(){
                this->stateContext.Update();
            }
    };
}

