#pragma once

#include <map>
#include <memory>
#include <queue>
#include <type_traits>

#include "filehandling.h"
#include "ieditable.h"
#include "ieditorstate.h"
#include "ieditorstatemutator.h"

using namespace Editor::Files;

namespace Editor::States{
    enum States{
        Normal = 0,
        Insert = 1
    };

    class IStateContextPasskey{
        protected:
            IStateContextPasskey(){};
    };


    class IStateContext : public IStateMutator, public std::enable_shared_from_this<IStateMutator>{
        public:
        map<string, std::shared_ptr<IState>> states;

        weak_ptr<IFileSaver> fileSaver;
        weak_ptr<IEditable> buffer;
        weak_ptr<ICommandManager> commandManager;
        weak_ptr<IUndoHandler> undoHandler;

        queue<int>* inputQueue;
        bool* quitToken;

        IStateContext(IStateContextPasskey passkey, 
                weak_ptr<IFileSaver> fileSaver, 
                weak_ptr<IEditable> buffer, 
                weak_ptr<ICommandManager> commandManager,
                weak_ptr<IUndoHandler> undoHandler,
                std::queue<int>* inputQueue, bool* quitToken) : 
        fileSaver(fileSaver), buffer(buffer), commandManager(commandManager), undoHandler(undoHandler), inputQueue(inputQueue), quitToken(quitToken){};

        virtual void Initialize(const string& defaultState) = 0;
        virtual void Update() = 0;

        template<typename S>
        requires std::is_base_of<IState, S>::value
        void AddState(){
            std::shared_ptr<IState> newState = 
                std::make_shared<S>(fileSaver, buffer, GetWeakPointer(), commandManager, undoHandler, inputQueue, quitToken);
            states[newState->StateName()] = std::move(newState);
        }

        virtual std::weak_ptr<IStateMutator> GetWeakPointer() = 0;
    };
}
