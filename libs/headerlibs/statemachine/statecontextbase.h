#pragma once

#include <string_view>

namespace StateMachines{
    class BaseStateContext{
        public:
            virtual void ChangeState(std::string_view name) = 0;
    };
}
