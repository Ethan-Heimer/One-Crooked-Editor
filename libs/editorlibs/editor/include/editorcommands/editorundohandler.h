#pragma once

#include <memory>
#include "editorcommandtype.h"

namespace Editor::Commands {
    class UndoHandler final{
        public: 
            UndoHandler();
            ~UndoHandler();

            void AddCommand(Command command);

            void UndoCommand();
            void RedoCommand(); 

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
