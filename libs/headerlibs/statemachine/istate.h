#pragma once

#include <functional>
#include <string>
#include <string_view>

namespace StateMachines{
    class IState{
        public:
            IState(std::function<void(std::string_view)> switchState) : SwitchState(switchState){};

            virtual constexpr std::string StateName() const = 0;

            virtual void OnEnter(){}
            virtual void OnUpdate() = 0;
            virtual void OnExit(){}

            virtual void Transition() = 0;

        protected:
            std::function<void(std::string_view)> SwitchState;
    };
}
