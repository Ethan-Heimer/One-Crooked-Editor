#pragma once

#include "editormutations/ieditormutatorbehavior.h"
#include "ieditable.h"
#include <memory>
#include <vector>

namespace CrookedEditor::Mutators {
    class InsertModeMutator : public Editor::Mutators::IMutatorBehavior {
        public:
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

                InsertCharacterAction(Editor::IEditable& buffer, char character) 
                    : ActionToken(buffer), addedCharacter(character) {

                    cursorRow = buffer.GetCurrentLineNumber(); 
                    cursorCol = buffer.GetCursorX();

                    buffer.InsertCharacter(character);
                };

                InsertCharacterAction(const InsertCharacterAction& other) = default;
                InsertCharacterAction(InsertCharacterAction&& other) = default;

                void UndoSelf() override {
                    buffer.GotoLine(cursorRow);
                    buffer.DeleteCharacterAt(cursorCol+1);
                }

                void RedoSelf() override {
                    buffer.GotoLine(cursorRow);
                    buffer.InsertCharacterAt(cursorCol, addedCharacter);
                }

                std::unique_ptr<ActionToken> Clone() const override{
                    return std::make_unique<InsertCharacterAction>(*this);
                }
            };

            struct DeleteCharacterAction : public ActionToken{
                char removedCharacter{};

                int cursorRow{};
                int cursorCol{};

                DeleteCharacterAction(Editor::IEditable& buffer) 
                    : ActionToken(buffer){

                    cursorRow = buffer.GetCurrentLineNumber(); 
                    cursorCol = buffer.GetCursorX();

                    removedCharacter = buffer.DeleteCharacter();
                };

                DeleteCharacterAction(const DeleteCharacterAction& other) = default;
                DeleteCharacterAction(DeleteCharacterAction&& other) = default;

                void UndoSelf(){
                    buffer.GotoLine(cursorRow);
                    buffer.InsertCharacterAt(cursorCol-1, removedCharacter);
                }

                void RedoSelf(){
                    buffer.GotoLine(cursorRow);
                    buffer.DeleteCharacterAt(cursorCol);
                }

                virtual std::unique_ptr<ActionToken> Clone() const{
                    return std::make_unique<DeleteCharacterAction>(*this);
                }
            };

            struct NewLineAction : public ActionToken{
                int cursorRow{};

                NewLineAction(Editor::IEditable& buffer) 
                    : ActionToken(buffer){

                    cursorRow = buffer.GetCurrentLineNumber();

                    buffer.InsertLine();
                    buffer.AppendTextToNextLine();
                };

                NewLineAction(const NewLineAction& other) = default;
                NewLineAction(NewLineAction&& other) = default;

                void UndoSelf(){
                    buffer.GotoLine(cursorRow + 1);
                    buffer.DeleteLine();
                }

                void RedoSelf(){
                    buffer.GotoLine(cursorRow);

                    buffer.InsertLine();
                    buffer.AppendTextToNextLine();
                }

                virtual std::unique_ptr<ActionToken> Clone() const{
                    return std::make_unique<NewLineAction>(*this);
                }
            };

            struct DeleteLineAction : public ActionToken{
                int cursorRow{};
                int textInsertedCol{};

                DeleteLineAction(Editor::IEditable& buffer) 
                    : ActionToken(buffer){

                    cursorRow = buffer.GetCurrentLineNumber();
                    buffer.DeleteLine();
                    textInsertedCol = buffer.GetCursorX();
                };

                DeleteLineAction(const DeleteLineAction& other) = default;
                DeleteLineAction(DeleteLineAction&& other) = default;

                void UndoSelf(){
                    buffer.GotoLine(cursorRow-1);

                    buffer.InsertLine();
                    buffer.MoveCursorToCol(textInsertedCol);
                    buffer.AppendTextToNextLine();
                }

                void RedoSelf(){
                    buffer.GotoLine(cursorRow);
                    buffer.DeleteLine();
                }

                virtual std::unique_ptr<ActionToken> Clone() const{
                    return std::make_unique<DeleteLineAction>(*this);
                }
            };

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
