#pragma once

#include <memory>
#include <queue>

#include "editorcommandmanager.h"
#include "editorfilehandling/ieditorfilehandler.h"
#include "ieditable.h"
#include "editorundohandler.h"
#include "istate.h"
#include "ieditorstatemutator.h"

using namespace StateMachines;
using namespace std;
using namespace Editor::Commands;

namespace Editor::States{
    class IState : public StateMachines::IState{
        public:
            IState(weak_ptr<FileHandling::IFileHandler> fileSaver, 
                    weak_ptr<IEditable> buffer, 
                    weak_ptr<IStateMutator> stateMutator, 
                    CommandManager& commandManager,
                    UndoHandler& undoHandler,
                    queue<int>* inputQueue, bool* quitToken)
                : stateMutator(stateMutator), inputQueue(inputQueue), 
                commandManager(commandManager), undoHandler(undoHandler), fileSaver(fileSaver), buffer(buffer), quitToken(quitToken){}

            virtual constexpr string StateName() const = 0;

        protected:
            weak_ptr<FileHandling::IFileHandler> fileSaver;
            weak_ptr<IEditable> buffer;
            weak_ptr<IStateMutator> stateMutator;
            CommandManager& commandManager;
            UndoHandler& undoHandler;

            queue<int>* inputQueue;
            bool* quitToken;
    };
}
