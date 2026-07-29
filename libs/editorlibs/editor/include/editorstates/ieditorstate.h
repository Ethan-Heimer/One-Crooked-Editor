#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <string_view>

#include "editorcommandmanager.h"
#include "editorfilehandler.h"
#include "ieditable.h"
#include "editorundohandler.h"
#include "istate.h"

using namespace Editor::Commands;

namespace Editor::States{
    class IEditorState : public StateMachines::IState{
        public:
            IEditorState(FileHandling::FileHandler& fileSaver, 
                    std::weak_ptr<IEditable> buffer, 
                    std::function<void(std::string_view)> switchState,
                    CommandManager& commandManager,
                    UndoHandler& undoHandler,
                    std::queue<int>* inputQueue, bool* quitToken)
                : StateMachines::IState(switchState), inputQueue(inputQueue), 
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
