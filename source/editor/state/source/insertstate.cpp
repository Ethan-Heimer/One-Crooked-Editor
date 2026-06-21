#include "insertstate.h"
#include <ncurses.h>

using namespace Editor::States;

void InsertState::OnUpdate(){
    int input = inputManager->GetKeyInput();
    nextState = Insert;

    if(input == ERR)
        return;

    if(input == KEY_BACKSPACE){
        if(editor->buffer->IsCursorAtBeginningOfLine()){
            editor->buffer->DeleteLine();
        }
        else
            editor->buffer->DeleteCharacter();
    } 
    else if(input == '\n' || input == '\r'){
        editor->buffer->InsertLine();
        editor->buffer->AppendTextToNextLine();
    } else if(input == 27){
        nextState = Normal;
    }
    else
        editor->buffer->InsertCharacter(input);

}

void InsertState::Transition(){
    if(nextState != States::Insert){
        context.ChangeState(nextState);
    }
}
