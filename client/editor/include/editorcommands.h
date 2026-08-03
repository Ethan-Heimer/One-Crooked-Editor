#pragma once

#include "editormutations/ieditormutatorbehavior.h"
#include "ieditable.h"
#include <memory>
#include <vector>

namespace CrookedEditor::Mutators {
    class InsertModeMutator : public Editor::Mutators::IMutatorBehavior {
        public:
            struct ActionToken{
                std::weak_ptr<Editor::IEditable> buffer{};
                ActionToken(std::weak_ptr<Editor::IEditable> buffer) : buffer(buffer){}
                virtual ~ActionToken(){};

                virtual void RedoSelf() = 0;
                virtual void UndoSelf() = 0;

                virtual std::unique_ptr<ActionToken> Clone() const = 0;
            };

            struct InsertCharacterAction : public ActionToken{
                char addedCharacter{};

                int cursorRow{};
                int cursorCol{};

                InsertCharacterAction(std::weak_ptr<Editor::IEditable> buffer, char character) 
                    : ActionToken(buffer), addedCharacter(character) {

                    cursorRow = buffer.lock()->GetCurrentLineNumber(); 
                    cursorCol = buffer.lock()->GetCursorX();

                    buffer.lock()->InsertCharacter(character);
                };

                InsertCharacterAction(const InsertCharacterAction& other) = default;
                InsertCharacterAction(InsertCharacterAction&& other) = default;

                void UndoSelf() override {
                    buffer.lock()->GotoLine(cursorRow);
                    buffer.lock()->DeleteCharacterAt(cursorCol+1);
                }

                void RedoSelf() override {
                    buffer.lock()->GotoLine(cursorRow);
                    buffer.lock()->InsertCharacterAt(cursorCol, addedCharacter);
                }

                std::unique_ptr<ActionToken> Clone() const override{
                    return std::make_unique<InsertCharacterAction>(*this);
                }
            };

            struct DeleteCharacterAction : public ActionToken{
                char removedCharacter{};

                int cursorRow{};
                int cursorCol{};

                DeleteCharacterAction(std::weak_ptr<Editor::IEditable> buffer) 
                    : ActionToken(buffer){

                    cursorRow = buffer.lock()->GetCurrentLineNumber(); 
                    cursorCol = buffer.lock()->GetCursorX();

                    removedCharacter = buffer.lock()->DeleteCharacter();
                };

                DeleteCharacterAction(const DeleteCharacterAction& other) = default;
                DeleteCharacterAction(DeleteCharacterAction&& other) = default;

                void UndoSelf(){
                    buffer.lock()->GotoLine(cursorRow);
                    buffer.lock()->InsertCharacterAt(cursorCol, removedCharacter);
                }

                void RedoSelf(){
                    buffer.lock()->GotoLine(cursorRow);
                    buffer.lock()->DeleteCharacterAt(cursorCol);
                }

                virtual std::unique_ptr<ActionToken> Clone() const{
                    return std::make_unique<DeleteCharacterAction>(*this);
                }
            };

            struct NewLineAction : public ActionToken{
                int cursorRow{};

                NewLineAction(std::weak_ptr<Editor::IEditable> buffer) 
                    : ActionToken(buffer){

                    cursorRow = buffer.lock()->GetCurrentLineNumber();

                    buffer.lock()->InsertLine();
                    buffer.lock()->AppendTextToNextLine();
                };

                NewLineAction(const NewLineAction& other) = default;
                NewLineAction(NewLineAction&& other) = default;

                void UndoSelf(){
                    buffer.lock()->GotoLine(cursorRow + 1);
                    buffer.lock()->DeleteLine();
                }

                void RedoSelf(){
                    buffer.lock()->GotoLine(cursorRow);

                    buffer.lock()->InsertLine();
                    buffer.lock()->AppendTextToNextLine();
                }

                virtual std::unique_ptr<ActionToken> Clone() const{
                    return std::make_unique<NewLineAction>(*this);
                }
            };

            struct DeleteLineAction : public ActionToken{
                int cursorRow{};

                DeleteLineAction(std::weak_ptr<Editor::IEditable> buffer) 
                    : ActionToken(buffer){

                    cursorRow = buffer.lock()->GetCurrentLineNumber();
                    buffer.lock()->DeleteLine();
                };

                DeleteLineAction(const DeleteLineAction& other) = default;
                DeleteLineAction(DeleteLineAction&& other) = default;

                void UndoSelf(){
                    buffer.lock()->GotoLine(cursorRow - 1);

                    buffer.lock()->InsertLine();
                    buffer.lock()->AppendTextToNextLine();
                }

                void RedoSelf(){
                    buffer.lock()->GotoLine(cursorRow);
                    buffer.lock()->DeleteLine();
                }

                virtual std::unique_ptr<ActionToken> Clone() const{
                    return std::make_unique<DeleteLineAction>(*this);
                }
            };

            InsertModeMutator(std::weak_ptr<Editor::IEditable>editable);
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
