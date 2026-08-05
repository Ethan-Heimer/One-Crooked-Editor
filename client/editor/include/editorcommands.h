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
                int cursorCol{};

                NewLineAction(Editor::IEditable& buffer) 
                    : ActionToken(buffer){

                    cursorRow = buffer.GetCurrentLineNumber();
                    cursorCol = buffer.GetCursorX();

                    buffer.InsertLine();

                    std::string textToAppend{};
                    buffer.DeleteFromCol(cursorCol, &textToAppend);
                    buffer.GotoNextLine();
                    buffer.InsertString(textToAppend);
                };

                NewLineAction(const NewLineAction& other) = default;
                NewLineAction(NewLineAction&& other) = default;

                void UndoSelf(){
                    buffer.GotoLine(cursorRow + 1);

                    std::string remainingText{};
                    buffer.DeleteLine(&remainingText);
                    buffer.InsertString(remainingText);
                }

                void RedoSelf(){
                    buffer.GotoLine(cursorRow);

                    buffer.InsertLine();

                    std::string textToAppend{};
                    buffer.DeleteFromCol(cursorCol, &textToAppend);
                    buffer.GotoNextLine();
                    buffer.InsertString(textToAppend);
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
                    std::string remainingText{};
                    buffer.DeleteLine(&remainingText);

                    textInsertedCol = buffer.GetCursorX();
                    buffer.InsertString(remainingText);
                    buffer.MoveCursorToCol(textInsertedCol);
                };

                DeleteLineAction(const DeleteLineAction& other) = default;
                DeleteLineAction(DeleteLineAction&& other) = default;

                void UndoSelf(){
                    buffer.GotoLine(cursorRow-1);

                    buffer.InsertLine();

                    std::string textToAppend{};
                    buffer.DeleteFromCol(textInsertedCol, &textToAppend);
                    buffer.GotoNextLine();
                    buffer.InsertString(textToAppend);
                }

                void RedoSelf(){
                    buffer.GotoLine(cursorRow);

                    std::string remainingText{};
                    buffer.DeleteLine(&remainingText);
                    buffer.InsertString(remainingText);
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
