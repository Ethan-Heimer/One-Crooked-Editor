#include "editorcommands/editorcommands.h"
#include <iostream>

using namespace Editor::Commands;

void TestCommand::Execute(){
    buffer.lock()->InsertCharacter('T');
}

void TestCommand::Undo(){
    buffer.lock()->DeleteCharacter();
}
