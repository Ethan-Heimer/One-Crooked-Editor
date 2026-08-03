#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <string_view>

#include "editormutations/editormutatormanager.h"
#include "editorfilehandler.h"
#include "ieditablecursorcommands.h"
#include "editormutations/editorundohandler.h"
#include "istate.h"

namespace Editor::States{
    class IEditorState : public StateMachines::IState{
        public:
            IEditorState(FileHandling::FileHandler& fileSaver, 
                    std::weak_ptr<IEditableCursorCommands> cursor, 
                    std::weak_ptr<const IEditableFileCommands> buffer, 
                    std::function<void(std::string_view)> switchState,
                    Mutators::MutatorManager& commandManager,
                    Mutators::UndoHandler& undoHandler,
                    std::queue<int>* inputQueue, bool* quitToken)
                : StateMachines::IState(switchState), inputQueue(inputQueue), 
                commandManager(commandManager), undoHandler(undoHandler), fileSaver(fileSaver), cursor(cursor), buffer(buffer), quitToken(quitToken){}

        protected:
            std::weak_ptr<IEditableCursorCommands> cursor;
            std::weak_ptr<const IEditableFileCommands> buffer;
            FileHandling::FileHandler& fileSaver;
            Mutators::MutatorManager& commandManager;
            Mutators::UndoHandler& undoHandler;

            std::queue<int>* inputQueue;
            bool* quitToken;
    };
}
