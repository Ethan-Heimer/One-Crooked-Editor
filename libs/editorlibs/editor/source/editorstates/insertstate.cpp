#include "insertstate.h"
#include <ncurses.h>

using namespace Editor::States;

constexpr string InsertState::StateName() const{
    return "Insert";
}

void InsertState::OnUpdate(){
    nextState = StateName();

    if(inputQueue->empty())
        return;

    int input = inputQueue->front();
    inputQueue->pop();

    if(input == KEY_BACKSPACE){
        if(buffer.lock()->IsCursorAtBeginningOfLine()){
            buffer.lock()->DeleteLine();
        }
        else
            buffer.lock()->DeleteCharacter();
    } 
    else if(input == '\n' || input == '\r'){
        buffer.lock()->InsertLine();
        buffer.lock()->AppendTextToNextLine();
    } else if(input == 27){
        nextState = "Normal";
    }
    else if(input == 9){
        for(int i = 0; i < 4; i++){
            buffer.lock()->InsertCharacter(' ');
        }
    }
    else
        buffer.lock()->InsertCharacter(input);

}

void InsertState::Transition(){
    if(nextState != StateName()){
        stateMutator.lock()->ChangeState(nextState);
    }
}
