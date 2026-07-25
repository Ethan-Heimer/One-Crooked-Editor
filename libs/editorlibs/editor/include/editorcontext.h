#pragma once

#include <memory>
#include <queue>
#include <string>
#include "editorstatecontext.h"
#include "editorundohandler.h"
#include "ieditable.h"
#include "editorcommandmanager.h"

namespace Editor{
    class EditorContext{
        public:
            Commands::UndoHandler undoHandler{};
            Commands::CommandManager commandManager{undoHandler};

            FileHandling::FileHandler fileHandler;

            States::StateContext stateContext{commandManager, fileHandler, undoHandler};

            std::shared_ptr<IEditable> buffer;

            bool quit;

            EditorContext( 
                    FileHandling::FileHandler fileHandler,
                    std::queue<int>* inputQueue, std::string fileName) : fileHandler(std::move(fileHandler)){

                this->fileHandler.fileName = fileName;

                this->buffer = this->fileHandler.LoadFromFile();

                this->commandManager.Initialize(this->buffer);
                this->stateContext.Initialize(buffer, "Normal", inputQueue, &quit);
            }

            void Update(){
                this->stateContext.Update();
            }
    };
}

