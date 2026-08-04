#pragma once

#include <functional>
#include <queue>
#include <string_view>

#include "editormutations/editormutatormanager.h"
#include "ieditablecursorcommands.h"
#include "editormutations/editorundohandler.h"
#include "istate.h"

namespace Editor::States{
    class IEditorState : public StateMachines::IState{
        public:
            IEditorState(IEditableCursorCommands& cursor, 
                    std::function<void()> saveBuffer,
                    std::function<void(std::string_view)> switchState,
                    Mutators::MutatorManager& commandManager,
                    Mutators::UndoHandler& undoHandler,
                    std::queue<int>* inputQueue, bool* quitToken)
                : StateMachines::IState(switchState), inputQueue(inputQueue), 
                commandManager(commandManager), undoHandler(undoHandler), SaveBuffer(saveBuffer), cursor(cursor), quitToken(quitToken){}

        protected:
            IEditableCursorCommands& cursor;
            std::function<void()> SaveBuffer;
            Mutators::MutatorManager& commandManager;
            Mutators::UndoHandler& undoHandler;

            std::queue<int>* inputQueue;
            bool* quitToken;
    };
}
