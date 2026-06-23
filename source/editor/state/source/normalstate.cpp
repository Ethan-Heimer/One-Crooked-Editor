#include "normalstate.h"
#include "iostream"
#include <ncurses.h>

using namespace Editor::States;

void NormalState::OnUpdate(){
    int input = inputManager.lock()->GetKeyInput();
    nextState = Normal;
    
    if(input == ERR){
        return;
    }

    if(input == ctrl('x'))
        editor.lock()->quit = true;
    else if(input == ctrl('w'))
        editor.lock()->Save();
    else if(input == ctrl('X')){
        editor.lock()->quit = true; 
        editor.lock()->Save();
    }
    else if(input == KEY_DOWN || input == 'j'){
        editor.lock()->buffer->GotoNextLine();
    } 
    else if(input == KEY_UP || input == 'k'){
        editor.lock()->buffer->GotoPreviousLine();
    }
    else if(input == KEY_LEFT || input == 'h'){
        editor.lock()->buffer->MoveCursorLeft();
    }
    else if(input == KEY_RIGHT || input == 'l'){
        editor.lock()->buffer->MoveCursorRight();
    } else if(input == 'i'){
        nextState = Insert;
    }
}

void NormalState::Transition(){
    if(nextState != States::Normal){
        context.lock()->ChangeState(nextState);
    }
};
