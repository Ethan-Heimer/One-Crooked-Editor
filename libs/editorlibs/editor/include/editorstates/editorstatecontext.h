#pragma once

#include <map>
#include <memory>
#include "editorfilehandling/ieditorfilehandler.h"
#include "ieditable.h"
#include "ieditorstate.h"
#include "ieditorstatemutator.h"
#include "statemachine.h"


namespace Editor::States{
    class StateContext final : public std::enable_shared_from_this<IStateMutator>, public IStateMutator{
        public:
            map<string, std::shared_ptr<IState>> states;

            weak_ptr<FileHandling::IFileHandler> fileSaver;
            weak_ptr<IEditable> buffer;
            CommandManager& commandManager;
            UndoHandler& undoHandler;

            queue<int>* inputQueue;
            bool* quitToken;

        StateContext(CommandManager& commandManager, UndoHandler& undoHandler) :
        commandManager(commandManager), undoHandler(undoHandler){};

        template<typename S>
        requires std::is_base_of<IState, S>::value
        void AddState(){
            std::shared_ptr<IState> newState = 
                std::make_shared<S>(fileSaver, buffer, GetWeakPointer(), commandManager, undoHandler, inputQueue, quitToken);
            states[newState->StateName()] = std::move(newState);
        }

        void Initialize(std::weak_ptr<IEditable> buffer, std::weak_ptr<FileHandling::IFileHandler> fileSaver, const string& defaultState, std::queue<int>* inputQueue, bool* quitToken);

        void Update();
        void ChangeState(const string state);

        std::weak_ptr<IStateMutator> GetWeakPointer();

        private:
            std::unique_ptr<StateMachines::StateMachine<IState>> stateMachine;
    };
}
