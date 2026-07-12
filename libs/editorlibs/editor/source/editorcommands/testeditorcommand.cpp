#include "editorcommands/editorcommands.h"
#include "ieditable.h"
#include "ieditorcommand.h"
#include "ieditorcommandscontainer.h"
#include <memory>

using namespace Editor::Commands;

TestCommand::TestCommand(weak_ptr<IEditable> editable, 
        weak_ptr<ICommandContainer> undoHandler, char character) 
    : ICommand(editable, undoHandler), character(character){};

void TestCommand::Initialize(){
    cursorPos = buffer.lock()->GetCursorX();
}

void TestCommand::Execute(){
    Initialize();
    Do(); 

    undoHandler.lock()->AddCommand(Clone());
}

void TestCommand::Do(){
    buffer.lock()->InsertCharacterAt(cursorPos, character);
}

void TestCommand::Undo(){
    buffer.lock()->DeleteCharacterAt(cursorPos+1);
}

std::unique_ptr<ICommand> TestCommand::Clone(){
    return make_unique<TestCommand>(*this);
}
