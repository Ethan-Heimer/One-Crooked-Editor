#pragma once

#include "editorcommands/ieditorcommand.h"


#define Command(Name) \
    class Name : public IEditorCommand { \
        public: \
            using IEditorCommand::IEditorCommand; \
            void Execute() override; \
            void Undo() override; \
    }; 

namespace Editor::Commands {
    Command(TestCommand);
}
