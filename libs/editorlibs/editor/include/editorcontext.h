#pragma once

#include <memory>
#include <queue>
#include <string>
#include "editorstatecontext.h"
#include "editormutations/editorundohandler.h"
#include "ieditable.h"
#include "editormutations/editormutatormanager.h"

namespace Editor{
    class EditorContext{
        public:
            Mutators::UndoHandler undoHandler{};
            Mutators::MutatorManager commandManager{undoHandler};

            FileHandling::FileHandler fileHandler;

            States::StateContext stateContext{commandManager, fileHandler, undoHandler};

            std::shared_ptr<IEditable> buffer;

            bool quit;

            EditorContext( 
                    FileHandling::FileHandler fileHandler, States::StateParameters stateParams,
                    std::queue<int>* inputQueue, std::string fileName) : fileHandler(std::move(fileHandler)){

                this->fileHandler.fileName = fileName;
                this->buffer = this->fileHandler.LoadFromFile();

                this->commandManager.Initialize(this->buffer);
                this->stateContext.Initialize(buffer, std::move(stateParams), inputQueue, &quit);
            }

            void Update(){
                this->stateContext.Update();
            }
    };
}

