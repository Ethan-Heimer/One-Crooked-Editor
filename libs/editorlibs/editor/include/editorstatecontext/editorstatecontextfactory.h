#pragma once

#include "ieditorstatecontext.h"
#include "ieditorstate.h"
#include "ieditorstatecontextfactory.h"

namespace Editor::States {
    template<typename T, typename... U> 
    requires std::is_base_of_v<IStateContext, T> && (std::is_base_of_v<IEditorState, U> && ...)
    class StateContextFactory;

    template<typename T, typename... U>
    class StateContextPasskey : public IStateContextPasskey{
        protected:
            StateContextPasskey() : IStateContextPasskey(){};
            friend class StateContextFactory<T, U...>;
    };
    
    template<typename T, typename... U> 
    requires std::is_base_of_v<IStateContext, T> && (std::is_base_of_v<IEditorState, U> && ...)
    class StateContextFactory : public IStateContextFactory{
        public:
            std::shared_ptr<IStateContext> Instanciate(std::weak_ptr<Buffers::IBufferFileHandler> fileHandler,
                    std::weak_ptr<Buffers::Buffer> buffer, queue<int>* inputQueue, bool* quitToken) override{

            std::shared_ptr<IStateContext> pointer = std::make_shared<T>
                (StateContextPasskey<T, U...>{}, fileHandler, buffer, inputQueue, quitToken);

            (pointer->AddState<U>(), ...);

            pointer->Initialize("Normal");

            return std::move(pointer);
        }
    };
}
