#pragma once

#include "editorcommands/ieditorcommand.h"
#include "ieditable.h"
#include "ieditorcommandscontainer.h"
#include <memory>


#define Command(Name) \
    class Name : public IEditorCommand { \
        public: \
            using IEditorCommand::IEditorCommand; \
            void Execute() override; \
            void Undo() override; \
    }; 

namespace Editor::Commands {
    class TestCommand : public IEditorCommand {
        public:
            TestCommand(std::weak_ptr<IEditable>editable, 
                    std::weak_ptr<IEditorCommandContainer> undoHandler, char character);

            void Execute() override;
            
            void Do() override;
            void Undo() override;

            std::unique_ptr<IEditorCommand> Clone() override;

        private:
            char character;
    }; 
}
