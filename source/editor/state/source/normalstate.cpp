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
        *quitToken = true;
    else if(input == ctrl('w'))
        fileHandler.lock()->WriteBufferToFile(buffer.lock());
    else if(input == ctrl('X')){
        *quitToken = true;
        fileHandler.lock()->WriteBufferToFile(buffer.lock());
    }
    else if(input == KEY_DOWN || input == 'j'){
        buffer.lock()->GotoNextLine();
    } 
    else if(input == KEY_UP || input == 'k'){
        buffer.lock()->GotoPreviousLine();
    }
    else if(input == KEY_LEFT || input == 'h'){
        buffer.lock()->MoveCursorLeft();
    }
    else if(input == KEY_RIGHT || input == 'l'){
        buffer.lock()->MoveCursorRight();
    } else if(input == 'i'){
        nextState = Insert;
    }
}

void NormalState::Transition(){
    if(nextState != States::Normal){
        context.lock()->ChangeState(nextState);
    }
};
