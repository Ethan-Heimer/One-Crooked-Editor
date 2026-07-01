#pragma once

#include "editorcommands/ieditorcommand.h"
#include "ieditable.h"
#include <memory>
#include <type_traits>

namespace Editor::Commands {
    class IEditorCommandManager{
        public:
            IEditorCommandManager(weak_ptr<IEditable> buffer) : buffer(buffer){};
            template<typename T, typename... U>
            requires std::is_base_of_v<IEditorCommand, T> 
            void Execute(U... args){
                auto command = std::make_shared<T>(buffer, args...);
                ExecuteCommand(command);
            }

            virtual void Undo() = 0;
            virtual void Redo() = 0;

        protected:
            std::weak_ptr<IEditable> buffer;
            virtual void ExecuteCommand(std::shared_ptr<IEditorCommand> command) = 0;  
    };
}
