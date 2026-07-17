#pragma once

#include "ieditorundohandler.h"
#include <memory>

namespace Editor::Commands {
    class UndoHandler final : public IUndoHandler{
        public: 
            UndoHandler();
            ~UndoHandler();

            void AddCommand(std::unique_ptr<ICommand> command) override;

            void UndoCommand() override;
            void RedoCommand() override; 

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
