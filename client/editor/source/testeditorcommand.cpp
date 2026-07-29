#include "editorcommands.h"

using namespace CrookedEditor::Commands;
using namespace std;

TestCommand::TestCommand(weak_ptr<Editor::IEditable> editable, char character) 
    : ICommandBehavior(editable), character(character){};

TestCommand::TestCommand(const TestCommand& other) : ICommandBehavior(other){
    cursorPos = other.cursorPos;
    character = other.character;
}

TestCommand::TestCommand(TestCommand&& other) : ICommandBehavior(other){
    cursorPos = other.cursorPos;
    character = other.character;
   
    other.cursorPos = 0;
    other.character = '\0';
}

void TestCommand::Initialize(){
    cursorPos = buffer.lock()->GetCursorX();
}

void TestCommand::Do(){
    buffer.lock()->InsertCharacterAt(cursorPos, character);
}

void TestCommand::Undo(){
    buffer.lock()->DeleteCharacterAt(cursorPos+1);
}
