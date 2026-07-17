#include "editorconstants.h"
#include "editorstates.h"
#include "editoractions/editoraction.h"

#include <ncurses.h>

using namespace Editor::States;

constexpr string NormalState::StateName() const{
    return Constants::NormalState;
}

void NormalState::OnEnter(){
    actions
    .AddAction("j", Action{[this](){ 
        buffer.lock()->GotoNextLine();
    }})
    .AddAction("k", Action{[this](){ 
        buffer.lock()->GotoPreviousLine();
    }})
    .AddAction("h", Action{[this](){ 
        buffer.lock()->MoveCursorLeft();
    }})
    .AddAction("l", Action{[this](){ 
        buffer.lock()->MoveCursorRight();
    }})
    .AddAction("i", Action{[this](){ 
        nextState = Constants::InsertState;
    }})
    .AddAction("o", Action{[this](){ 
        nextState = Constants::InsertState;
        buffer.lock()->InsertLine();
        buffer.lock()->GotoNextLine();
    }})
    .AddAction("u", Action{[this](){ 
        undoHandler.lock()->UndoCommand();
    }})
    .AddAction("r", Action{[this](){ 
        undoHandler.lock()->RedoCommand();
    }})
    .AddAction(":w", Action{[this](){ 
        fileSaver.lock()->SaveToFile(buffer.lock());
    }})
    .AddAction(":q", Action{[this](){ 
        *quitToken = true;
    }})
    .AddAction(":wq", Action{[this](){ 
        *quitToken = true;
        fileSaver.lock()->SaveToFile(buffer.lock());
    }});
}

void NormalState::OnUpdate(){
    nextState = StateName();
    if(inputQueue->empty())
        return;

    int input = inputQueue->front();    
    inputQueue->pop();

    actions.TraverseToNextAction(static_cast<char>(input));
}

void NormalState::Transition(){
    if(nextState != StateName()){
        stateMutator.lock()->ChangeState(nextState);
    }
};
