#pragma once

#include "editorcommands/ieditorcommand.h"
#include <memory>
#include "ieditable.h"
#include "ieditorcommandmanager.h"

namespace Editor::Commands {
    class EditorCommandManager : public IEditorCommandManager{
        public:
            EditorCommandManager(std::weak_ptr<IEditable> buffer);
            ~EditorCommandManager();

            void Undo() override;
            void Redo() override;

        protected:
            void ExecuteCommand(std::shared_ptr<IEditorCommand> command) override;

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
