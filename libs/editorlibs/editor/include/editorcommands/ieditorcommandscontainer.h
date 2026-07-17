#pragma once

#include <memory>
#include <type_traits>

namespace  Editor::Commands{
    class ICommand;
    class ICommandContainer{
        public:
            template<typename T>
            requires std::is_base_of_v<ICommand, T>
            void AddCommand(T command){
                AddCommand(std::make_unique<T>(command));
            };

            virtual void AddCommand(std::unique_ptr<ICommand> command) = 0;
    };
}
