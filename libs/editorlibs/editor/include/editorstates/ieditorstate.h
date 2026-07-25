#pragma once

#include <memory>
#include <queue>

#include "editorcommandmanager.h"
#include "editorfilehandling/ieditorfilehandler.h"
#include "ieditable.h"
#include "editorundohandler.h"
#include "istate.h"
#include "statecontextbase.h"

using namespace Editor::Commands;

namespace Editor::States{
    class IEditorState : public StateMachines::IState{
        public:
            IEditorState(FileHandling::FileHandler& fileSaver, 
                    std::weak_ptr<IEditable> buffer, 
                    StateMachines::BaseStateContext& stateContext, 
                    CommandManager& commandManager,
                    UndoHandler& undoHandler,
                    std::queue<int>* inputQueue, bool* quitToken)
                : StateMachines::IState(stateContext), inputQueue(inputQueue), 
                commandManager(commandManager), undoHandler(undoHandler), fileSaver(fileSaver), buffer(buffer), quitToken(quitToken){}

        protected:
            std::weak_ptr<IEditable> buffer;
            FileHandling::FileHandler& fileSaver;
            CommandManager& commandManager;
            UndoHandler& undoHandler;

            std::queue<int>* inputQueue;
            bool* quitToken;
    };
}
