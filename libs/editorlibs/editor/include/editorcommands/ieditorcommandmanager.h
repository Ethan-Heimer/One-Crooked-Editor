#pragma once

#include "editorcommands/ieditorcommand.h"
#include "ieditable.h"
#include "ieditorundohandler.h"
#include <memory>
#include <type_traits>

namespace Editor::Commands {
    class IEditorCommandManager{
        public:
            IEditorCommandManager(weak_ptr<IEditable> buffer, 
                    weak_ptr<IEditorCommandUndoHandler> undoHandler) : buffer(buffer), undoHandler(undoHandler){};

            template<typename T, typename... U>
            requires std::is_base_of_v<IEditorCommand, T> 
            T CreateCommand(U... args){
                return T{buffer, this->undoHandler, args...};
            }

        protected:
            weak_ptr<IEditorCommandUndoHandler> undoHandler;
            weak_ptr<IEditable> buffer;
    };
}
