#pragma once

#include <functional>
#include <string_view>

#include "editormutations/editormutatormanager.h"
#include "ieditablecursorcommands.h"
#include "editormutations/editorundohandler.h"
#include "istate.h"

namespace Editor::States{
    class IEditorState : public StateMachines::IState{
        public:
            IEditorState(std::function<void(std::string_view)> switchState,
                    IEditableCursorCommands& cursor, 
                    std::function<void()> saveBuffer, 
                    Mutators::MutatorManager& mutationManager,
                    Mutators::UndoHandler& undoHandler,
                    std::string_view& input, bool* quitToken)
                : StateMachines::IState(switchState), cursor(cursor), SaveBuffer(saveBuffer), 
                mutationManager(mutationManager),undoHandler(undoHandler), input(input), quitToken(quitToken){}

        protected:
            IEditableCursorCommands& cursor;
            std::function<void()> SaveBuffer;
            Mutators::MutatorManager& mutationManager;
            Mutators::UndoHandler& undoHandler;

            std::string_view& input;
            bool* quitToken;
    };
}
