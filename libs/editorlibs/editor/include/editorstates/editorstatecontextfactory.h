#pragma once

#include "filehandling.h"
#include "ieditable.h"
#include "ieditorstatecontext.h"
#include "ieditorstate.h"
#include "ieditorstatecontextfactory.h"
#include "ieditorundohandler.h"

namespace Editor::States {
    template<typename T, typename... U> 
    requires std::is_base_of_v<IStateContext, T> && (std::is_base_of_v<IState, U> && ...)
    class StateContextFactory;

    template<typename T, typename... U>
    class StateContextPasskey : public IStateContextPasskey{
        protected:
            StateContextPasskey() : IStateContextPasskey(){};
            friend class StateContextFactory<T, U...>;
    };
    
    template<typename T, typename... U> 
    requires std::is_base_of_v<IStateContext, T> && (std::is_base_of_v<IState, U> && ...)
    class StateContextFactory : public IStateContextFactory{
        public:
            std::shared_ptr<IStateContext> Instanciate(std::weak_ptr<IFileSaver> fileSaver,
                    std::weak_ptr<IEditable> buffer, 
                    weak_ptr<ICommandManager> commandManager,
                    weak_ptr<IUndoHandler> undoHandler,
                    queue<int>* inputQueue, bool* quitToken) override{

            std::shared_ptr<IStateContext> pointer = std::make_shared<T>
                (StateContextPasskey<T, U...>{}, fileSaver, buffer, commandManager, undoHandler, inputQueue, quitToken);

            (pointer->AddState<U>(), ...);

            pointer->Initialize("Normal");

            return std::move(pointer);
        }
    };
}
