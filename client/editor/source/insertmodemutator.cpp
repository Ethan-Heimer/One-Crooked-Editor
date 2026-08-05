#include "editorcommands.h"
#include <memory>

using namespace CrookedEditor::Mutators;
using namespace std;


InsertCharacterAction::InsertCharacterAction(Editor::IEditable& buffer, char character)
    : ActionToken(buffer), addedCharacter(character) {

    cursorRow = buffer.GetCurrentLineNumber(); 
    cursorCol = buffer.GetCursorX();

    buffer.InsertCharacter(character);
};

InsertCharacterAction::InsertCharacterAction(const InsertCharacterAction& other) = default;
InsertCharacterAction::InsertCharacterAction(InsertCharacterAction&& other) = default;

void InsertCharacterAction::UndoSelf(){
    buffer.GotoLine(cursorRow);
    buffer.DeleteCharacterAt(cursorCol+1);
}

void InsertCharacterAction::RedoSelf(){
    buffer.GotoLine(cursorRow);
    buffer.InsertCharacterAt(cursorCol, addedCharacter);
}

std::unique_ptr<ActionToken> InsertCharacterAction::Clone() const{
    return std::make_unique<InsertCharacterAction>(*this);
}


DeleteCharacterAction::DeleteCharacterAction(Editor::IEditable& buffer) : ActionToken(buffer){
    cursorRow = buffer.GetCurrentLineNumber(); 
    cursorCol = buffer.GetCursorX();

    removedCharacter = buffer.DeleteCharacter();
};

DeleteCharacterAction::DeleteCharacterAction(const DeleteCharacterAction& other) = default;
DeleteCharacterAction::DeleteCharacterAction(DeleteCharacterAction&& other) = default;

void DeleteCharacterAction::UndoSelf(){
    buffer.GotoLine(cursorRow);
    buffer.InsertCharacterAt(cursorCol-1, removedCharacter);
}

void DeleteCharacterAction::RedoSelf(){
    buffer.GotoLine(cursorRow);
    buffer.DeleteCharacterAt(cursorCol);
}

std::unique_ptr<ActionToken> DeleteCharacterAction::Clone() const{
    return std::make_unique<DeleteCharacterAction>(*this);
}

NewLineAction::NewLineAction(Editor::IEditable& buffer) : ActionToken(buffer){
    cursorRow = buffer.GetCurrentLineNumber();
    cursorCol = buffer.GetCursorX();

    buffer.InsertLine();

    std::string textToAppend{};
    buffer.DeleteFromCol(cursorCol, &textToAppend);
    buffer.GotoNextLine();
    buffer.InsertString(textToAppend);
};

NewLineAction::NewLineAction(const NewLineAction& other) = default;
NewLineAction::NewLineAction(NewLineAction&& other) = default;

void NewLineAction::UndoSelf(){
    buffer.GotoLine(cursorRow + 1);

    std::string remainingText{};
    buffer.DeleteLine(&remainingText);
    buffer.InsertString(remainingText);
}

void NewLineAction::RedoSelf(){
    buffer.GotoLine(cursorRow);

    buffer.InsertLine();

    std::string textToAppend{};
    buffer.DeleteFromCol(cursorCol, &textToAppend);
    buffer.GotoNextLine();
    buffer.InsertString(textToAppend);
}

std::unique_ptr<ActionToken> NewLineAction::Clone() const{
    return std::make_unique<NewLineAction>(*this);
}

DeleteLineAction::DeleteLineAction(Editor::IEditable& buffer) : ActionToken(buffer){
    cursorRow = buffer.GetCurrentLineNumber();
    std::string remainingText{};
    buffer.DeleteLine(&remainingText);

    textInsertedCol = buffer.GetCursorX();
    buffer.InsertString(remainingText);
    buffer.MoveCursorToCol(textInsertedCol);
};

DeleteLineAction::DeleteLineAction(const DeleteLineAction& other) = default;
DeleteLineAction::DeleteLineAction(DeleteLineAction&& other) = default;

void DeleteLineAction::UndoSelf(){
    buffer.GotoLine(cursorRow-1);

    buffer.InsertLine();

    std::string textToAppend{};
    buffer.DeleteFromCol(textInsertedCol, &textToAppend);
    buffer.GotoNextLine();
    buffer.InsertString(textToAppend);
}

void DeleteLineAction::RedoSelf(){
    buffer.GotoLine(cursorRow);

    std::string remainingText{};
    buffer.DeleteLine(&remainingText);
    buffer.InsertString(remainingText);
}

std::unique_ptr<ActionToken> DeleteLineAction::Clone() const{
    return std::make_unique<DeleteLineAction>(*this);
}

InsertModeMutator::InsertModeMutator(Editor::IEditable& editable) 
    : IMutatorBehavior(editable){};

// this is copied twice: I think this is because of the type erosion
InsertModeMutator::InsertModeMutator(const InsertModeMutator& other) : IMutatorBehavior(other){
    for(int i = 0; i < other.undoStack.size(); i++){
        this->undoStack.push_back(other.undoStack[i]->Clone());
    }
}

InsertModeMutator::InsertModeMutator(InsertModeMutator&& other) : IMutatorBehavior(other){
    undoStack = std::move(other.undoStack);
}

void InsertModeMutator::Do(){
    if(undoStack.size() == 0)
        return;

    for(int i = undoStack.size()-1; i >= 0; i--){
        undoStack[i]->RedoSelf();
    }
}

void InsertModeMutator::Undo(){
    if(undoStack.size() == 0)
        return;

    for(int i = 0; i < undoStack.size(); i++){
        undoStack[i]->UndoSelf();
    }
}
