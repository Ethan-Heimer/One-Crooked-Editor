#pragma once

#include <concepts>
#include <memory>
#include "loststateexception.h"

#include "istate.h"

namespace StateMachines{
    template<typename T>
    requires std::derived_from<T, IState>
    class StateMachine{
        public:
            StateMachine(std::shared_ptr<T> defaultState) : currentState(defaultState){
                currentState->OnEnter();
            }

            void SwitchState(std::shared_ptr<T> newState){
                currentState->OnExit();
                currentState = newState;
                currentState->OnEnter();
            }

            void Update(){
                if(currentState == nullptr)
                    throw LostStateException("A null (lost) state was encountered while updating a state machine.");

                currentState->OnUpdate();
                currentState->Transition();
            }
        private:
            std::shared_ptr<T> currentState;
    };

    using UniversalStateMachine = StateMachine<IState>;
}
