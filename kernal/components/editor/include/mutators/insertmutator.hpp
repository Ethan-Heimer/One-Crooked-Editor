#pragma once

#include "editormutations/ieditormutatorbehavior.h"
#include "ieditable.h"
#include <memory>
#include <vector>

namespace CrookedEditor::Mutators {
    struct ActionToken{
        Editor::IEditable& buffer;

        ActionToken(Editor::IEditable& buffer) : buffer(buffer){}
        virtual ~ActionToken(){};
        virtual void RedoSelf() = 0;
        virtual void UndoSelf() = 0;

        virtual std::unique_ptr<ActionToken> Clone() const = 0;
    };

    struct InsertCharacterAction : public ActionToken{
        char addedCharacter{};

        int cursorRow{};
        int cursorCol{};

        InsertCharacterAction(Editor::IEditable& buffer, char character);
        InsertCharacterAction(const InsertCharacterAction& other);
        InsertCharacterAction(InsertCharacterAction&& other);

        void UndoSelf() override;
        void RedoSelf() override;
        std::unique_ptr<ActionToken> Clone() const override;
    };

    struct DeleteCharacterAction : public ActionToken{
        char removedCharacter{};

        int cursorRow{};
        int cursorCol{};

        DeleteCharacterAction(Editor::IEditable& buffer);
        DeleteCharacterAction(const DeleteCharacterAction& other);
        DeleteCharacterAction(DeleteCharacterAction&& other);

        void UndoSelf() override;
        void RedoSelf() override;
        virtual std::unique_ptr<ActionToken> Clone() const override;
    };

    struct NewLineAction : public ActionToken{
        int cursorRow{};
        int cursorCol{};

        NewLineAction(Editor::IEditable& buffer);
        NewLineAction(const NewLineAction& other);
        NewLineAction(NewLineAction&& other);

        void UndoSelf() override;
        void RedoSelf() override;
        std::unique_ptr<ActionToken> Clone() const override;
    };

    struct DeleteLineAction : public ActionToken{
        int cursorRow{};
        int textInsertedCol{};

        DeleteLineAction(Editor::IEditable& buffer);
        DeleteLineAction(const DeleteLineAction& other);
        DeleteLineAction(DeleteLineAction&& other);

        void UndoSelf() override;
        void RedoSelf() override;

        std::unique_ptr<ActionToken> Clone() const override;
    };

    class InsertModeMutator : public Editor::Mutators::IMutatorBehavior {
        public:
            InsertModeMutator(Editor::IEditable& editable);
            InsertModeMutator(const InsertModeMutator& other);
            InsertModeMutator(InsertModeMutator&& other);
 
            template<typename T, typename... U>
            void DoAction(U... args){
                std::unique_ptr<ActionToken> action = std::make_unique<T>(buffer, args...);
                undoStack.insert(undoStack.begin(), std::move(action));
            }

            void Do() override;
            void Undo() override;

        private:
            std::vector<std::unique_ptr<ActionToken>> undoStack{};
    };     
}
