#pragma once

#include <memory>
#include <type_traits>

namespace  Editor::Commands{
    class IEditorCommand;
    class IEditorCommandContainer{
        public:
            template<typename T>
            requires std::is_base_of_v<IEditorCommand, T>
            void AddCommand(T command){
                AddCommand(std::make_unique<T>(command));
            };

            virtual void AddCommand(std::unique_ptr<IEditorCommand> command) = 0;
    };
}
