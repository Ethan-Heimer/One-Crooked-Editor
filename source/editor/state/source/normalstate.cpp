#include "normalstate.h"
#include "iostream"
#include <ncurses.h>

using namespace Editor::States;

void NormalState::OnUpdate(){
    int input = inputManager->GetKeyInput();
    nextState = Normal;
    
    if(input == ERR){
        return;
    }

    if(input == ctrl('x'))
        editor->quit = true;
    else if(input == ctrl('w'))
        editor->Save();
    else if(input == ctrl('X')){
        editor->quit = true; 
        editor->Save();
    }
    else if(input == KEY_DOWN){
        editor->buffer->GotoNextLine();
    } 
    else if(input == KEY_UP){
        editor->buffer->GotoPreviousLine();
    }
    else if(input == KEY_LEFT){
        editor->buffer->MoveCursorLeft();
    }
    else if(input == KEY_RIGHT){
        editor->buffer->MoveCursorRight();
    } else if(input == 'i'){
        nextState = Insert;
    } else 
        editor->buffer->InsertCharacter(input);
}

void NormalState::Transition(){
    if(nextState != States::Normal){
        context.ChangeState(nextState);
    }
};
