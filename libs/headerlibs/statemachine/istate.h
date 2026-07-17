#pragma once

namespace StateMachines{
    class IState{
        public:
            virtual void OnEnter(){}
            virtual void OnUpdate() = 0;
            virtual void OnExit(){}

            virtual void Transition() = 0;
    };
}
