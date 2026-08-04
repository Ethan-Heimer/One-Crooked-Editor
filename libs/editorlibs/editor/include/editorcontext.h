#pragma once

#include <memory>
#include <queue>
#include <string>
#include "editorfilehandler.h"
#include "editorstatecontext.h"
#include "editorundohandler.h"
#include "ieditable.h"
#include "editormutatormanager.h"

namespace Editor{
    class EditorContext{
        public:
            FileHandling::FileHandler fileHandler;

            std::unique_ptr<Mutators::UndoHandler> undoHandler;
            std::unique_ptr<Mutators::MutatorManager> mutationManager;
            std::unique_ptr<States::StateContext> stateContext;

            std::shared_ptr<IEditable> buffer;

            bool quit;

            EditorContext(FileHandling::FileHandler fileHandler, States::StateParameters stateParams,
                    std::queue<int>* inputQueue, std::string fileName);

            void Update();
    };
}

