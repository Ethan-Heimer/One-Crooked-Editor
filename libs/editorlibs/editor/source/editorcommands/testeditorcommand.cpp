#include "editorcommands/editorcommands.h"
#include "ieditable.h"
#include "ieditorcommand.h"
#include "ieditorcommandscontainer.h"
#include <memory>

using namespace Editor::Commands;

TestCommand::TestCommand(weak_ptr<IEditable> editable, 
        weak_ptr<IEditorCommandContainer> undoHandler, char character) 
    : IEditorCommand(editable, undoHandler), character(character){};

void TestCommand::Execute(){
    Do(); 

    undoHandler.lock()->AddCommand(Clone());
}

void TestCommand::Do(){
    buffer.lock()->InsertCharacter(character);
}

void TestCommand::Undo(){
    buffer.lock()->DeleteCharacter();
}

std::unique_ptr<IEditorCommand> TestCommand::Clone(){
    return make_unique<TestCommand>(buffer, undoHandler, character);
}
