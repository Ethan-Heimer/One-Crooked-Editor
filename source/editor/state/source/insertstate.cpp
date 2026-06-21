#include "insertstate.h"
#include <ncurses.h>

using namespace Editor::States;

void InsertState::OnUpdate(){
    int input = inputManager.lock()->GetKeyInput();
    nextState = Insert;

    if(input == ERR)
        return;

    if(input == KEY_BACKSPACE){
        if(editor.lock()->buffer->IsCursorAtBeginningOfLine()){
            editor.lock()->buffer->DeleteLine();
        }
        else
            editor.lock()->buffer->DeleteCharacter();
    } 
    else if(input == '\n' || input == '\r'){
        editor.lock()->buffer->InsertLine();
        editor.lock()->buffer->AppendTextToNextLine();
    } else if(input == 27){
        nextState = Normal;
    }
    else
        editor.lock()->buffer->InsertCharacter(input);

}

void InsertState::Transition(){
    if(nextState != States::Insert){
        context.lock()->ChangeState(nextState);
    }
}
