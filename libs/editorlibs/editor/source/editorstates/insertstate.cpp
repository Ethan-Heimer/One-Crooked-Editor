#include <ncurses.h>

#include "editorstates.h"
#include "editorconstants.h"

using namespace Editor::States;

constexpr string InsertState::StateName() const{
    return Constants::InsertState;
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
        nextState = Constants::NormalState;
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
