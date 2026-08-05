#include "editorcommands.h"
#include <memory>

using namespace CrookedEditor::Mutators;
using namespace std;

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
