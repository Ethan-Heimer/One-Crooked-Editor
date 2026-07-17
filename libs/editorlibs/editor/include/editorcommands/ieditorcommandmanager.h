#pragma once

#include "editorcommands/ieditorcommand.h"
#include "ieditable.h"
#include "ieditorundohandler.h"
#include <memory>
#include <type_traits>

namespace Editor::Commands {
    class ICommandManager{
        public:
            ICommandManager(weak_ptr<IEditable> buffer, 
                    weak_ptr<IUndoHandler> undoHandler) : buffer(buffer), undoHandler(undoHandler){};

            template<typename T, typename... U>
            requires std::is_base_of_v<ICommand, T> 
            T CreateCommand(U... args){
                return T{buffer, this->undoHandler, args...};
            }

        protected:
            weak_ptr<IUndoHandler> undoHandler;
            weak_ptr<IEditable> buffer;
    };
}
