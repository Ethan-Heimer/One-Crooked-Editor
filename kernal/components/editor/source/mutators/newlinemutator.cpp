#include "newlinemutator.hpp"
#include "ieditable.h"
#include "ieditormutatorbehavior.h"

using namespace CrookedEditor::Mutators;
using namespace Editor::Mutators;

NewLineMutator::NewLineMutator(Editor::IEditable& buffer) 
    : Editor::Mutators::IMutatorBehavior(buffer){
    lineNumber = buffer.GetCurrentLineNumber();
};

NewLineMutator::NewLineMutator(const NewLineMutator& other) : IMutatorBehavior(other){
    lineNumber = other.lineNumber;
}

NewLineMutator::NewLineMutator(NewLineMutator&& other) : IMutatorBehavior(other){
    lineNumber = other.lineNumber;
    other.lineNumber = 0;
}

void NewLineMutator::Do(){
    buffer.GotoLine(lineNumber);
    buffer.InsertLine();
    buffer.GotoNextLine();
}

void NewLineMutator::Undo(){
    buffer.GotoLine(lineNumber + 1);
    buffer.DeleteLine(nullptr);
    buffer.GotoLine(lineNumber);
}
