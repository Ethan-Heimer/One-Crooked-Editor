#pragma once

#include <memory>
#include <ieditable.h>
#include "editorcommandtype.h"
#include "editorundohandler.h"
#include "ieditorcommand.h"

namespace Editor::Commands {
    class CommandManager {
        public:
            CommandManager(UndoHandler& undoHandler) : undoHandler(undoHandler){};

            void Initialize(std::weak_ptr<IEditable> buffer){
                this->buffer = buffer;
            };

            template<typename T, typename... U>
            requires std::is_base_of_v<ICommandBehavior, T> 
            Command CreateCommand(U... args){
                auto addToUndo = [this](Command command){
                    this->undoHandler.AddCommand(std::move(command));
                };

                return Command{addToUndo, T{buffer, args...}};
            }

        protected:
            UndoHandler& undoHandler;
            std::weak_ptr<IEditable> buffer;
    };
}
