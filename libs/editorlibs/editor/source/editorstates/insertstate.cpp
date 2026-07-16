#include <cctype>
#include <ncurses.h>

#include "editorstates.h"
#include "editorconstants.h"

using namespace Editor::States;

constexpr string InsertState::StateName() const{
    return Constants::InsertState;
}

void InsertState::OnEnter(){
    actions
    .AddAction("\b", Action{[this](){
        if(buffer.lock()->IsCursorAtBeginningOfLine()){
            buffer.lock()->DeleteLine();
        }
        else
            buffer.lock()->DeleteCharacter();
    }})
    .AddAction("\n", Action{[this](){
        buffer.lock()->InsertLine();
        buffer.lock()->AppendTextToNextLine();
    }})
    .AddAction("\r", Action{[this](){
        buffer.lock()->InsertLine();
        buffer.lock()->AppendTextToNextLine();
    }})
    .AddAction("\x1b", Action{[this](){
        nextState = Constants::NormalState;
    }})
    .AddAction("\t", Action{[this](){
        for(int i = 0; i < 4; i++){
            buffer.lock()->InsertCharacter(' ');
        }
    }});

}

void InsertState::OnUpdate(){
    nextState = StateName();

    if(inputQueue->empty())
        return;

    int input = inputQueue->front();
    inputQueue->pop();

    actions.TraverseToNextAction(static_cast<char>(input));

    if(std::isprint(static_cast<unsigned char>(input)))
        buffer.lock()->InsertCharacter(input);
}

void InsertState::Transition(){
    if(nextState != StateName()){
        stateMutator.lock()->ChangeState(nextState);
    }
}
