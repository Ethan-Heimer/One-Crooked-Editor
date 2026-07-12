#pragma once

#include "ieditorundohandler.h"
#include <memory>

namespace Editor::Commands {
    class EditorUndoHandler final : public IEditorCommandUndoHandler{
        public: 
            EditorUndoHandler();
            ~EditorUndoHandler();

            void AddCommand(std::unique_ptr<IEditorCommand> command) override;

            void UndoCommand() override;
            void RedoCommand() override; 

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
