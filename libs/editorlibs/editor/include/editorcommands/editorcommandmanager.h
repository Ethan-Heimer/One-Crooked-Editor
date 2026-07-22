#pragma once

#include <memory>
#include <ieditable.h>
#include "editorundohandler.h"

namespace Editor::Commands {
    class CommandManager {
        public:
            CommandManager(UndoHandler& undoHandler) : undoHandler(undoHandler){};

            void Initialize(std::weak_ptr<IEditable> buffer){
                this->buffer = buffer;
            };

            template<typename T, typename... U>
            requires std::is_base_of_v<ICommand, T> 
            T CreateCommand(U... args){
                return T{buffer, this->undoHandler, args...};
            }

        protected:
            UndoHandler& undoHandler;
            std::weak_ptr<IEditable> buffer;
    };
}
