#pragma once

#include <string>
namespace Editor::States{
    class IStateMutator{
        public:
            virtual void ChangeState(const std::string name) = 0;
    };
}
