#pragma once

#include "ieditorcommand.h"
#include "ieditable.h"
#include <memory>

#define Command(Name) \
    class Name : public IEditorCommand { \
        public: \
            using IEditorCommand::IEditorCommand; \
            void Execute() override; \
            void Undo() override; \
    }; 

namespace CrookedEditor::Commands {
    class TestCommand : public Editor::Commands::ICommandBehavior {
        public:
            TestCommand(std::weak_ptr<Editor::IEditable>editable, char character);
            TestCommand(const TestCommand& other);
            TestCommand(TestCommand&& other);

            void Initialize() override;
            
            void Do() override;
            void Undo() override;
        private:
            unsigned int cursorPos{};
            char character{};
    }; 
}
