#pragma once

#include <type_traits>
#include <memory>
#include "ieditor.h"
#include "iinputmanager.h"

namespace Editor::States{
    enum States{
        Normal = 0,
        Insert = 1
    };

    class StateContextFactory;
    class StateContextPasskey{
        private:
            StateContextPasskey(){};
            friend class StateContextFactory;
    };

    class IStateContext{
        public:
        IStateContext(StateContextPasskey passkey){};

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void ChangeState(States state) = 0;
    };

    class StateContextFactory{
        public:
        template<typename T, typename... U> 
        requires std::is_base_of<IStateContext, T>::value

        std::shared_ptr<IStateContext> Instanciate(std::weak_ptr<IEditor> editor, 
                weak_ptr<Systems::Input::IInputManager> inputManager, U... args){
            std::shared_ptr<IStateContext> pointer = std::make_shared<T>(StateContextPasskey{}, editor, inputManager, args...);
            pointer->Initialize();

            return std::move(pointer);
        }
    };
}
