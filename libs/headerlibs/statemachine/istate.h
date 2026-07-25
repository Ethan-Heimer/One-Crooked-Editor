#pragma once

#include "statecontextbase.h"
namespace StateMachines{
    class IState{
        public:
            IState(BaseStateContext& stateContext) : stateContext(stateContext){};

            virtual constexpr std::string StateName() const = 0;

            virtual void OnEnter(){}
            virtual void OnUpdate() = 0;
            virtual void OnExit(){}

            virtual void Transition() = 0;

        protected:
            BaseStateContext& stateContext;
    };
}
