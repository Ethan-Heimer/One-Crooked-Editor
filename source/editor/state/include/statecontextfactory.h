#pragma once

#include "icontext.h"
#include "istatecontextfactory.h"

namespace Editor::States {
    template<typename T> 
    requires std::is_base_of<IStateContext, T>::value
    class StateContextFactory;

    template<typename T>
    class StateContextPasskey : public IStateContextPasskey{
        protected:
            StateContextPasskey() : IStateContextPasskey(){};
            friend class StateContextFactory<T>;
    };
    
    template<typename T> 
    requires std::is_base_of<IStateContext, T>::value
    class StateContextFactory : public IStateContextFactory{
        public:
            std::shared_ptr<IStateContext> Instanciate(std::weak_ptr<Buffers::IBufferFileHandler> fileHandler,
                    std::weak_ptr<Buffers::Buffer> buffer, weak_ptr<Systems::Input::IInputManager> inputManager, bool* quitToken) override{
            std::shared_ptr<IStateContext> pointer = std::make_shared<T>(StateContextPasskey<T>{});
            pointer->Initialize(fileHandler, buffer, inputManager, quitToken);

            return std::move(pointer);
        }
    };
}
