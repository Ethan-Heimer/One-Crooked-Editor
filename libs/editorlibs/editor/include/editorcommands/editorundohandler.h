#pragma once

#include <memory>
#include "ieditorcommand.h"

namespace Editor::Commands {
    class UndoHandler final{
        public: 
            UndoHandler();
            ~UndoHandler();

            void AddCommand(std::unique_ptr<ICommand> command);

            void UndoCommand();
            void RedoCommand(); 

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
