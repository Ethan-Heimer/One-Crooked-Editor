#include "editorcommands.h"
#include "editorconstants.h"
#include "editorstates.h"
#include "editoractions/editoraction.h"

#include <ncurses.h>

using namespace CrookedEditor::States;
using namespace CrookedEditor::Commands;

constexpr string NormalState::StateName() const{
    return Constants::NormalState;
}

void NormalState::OnEnter(){
    actions
    .AddAction("j", [this](){ 
        buffer.lock()->GotoNextLine();
    })
    .AddAction("k", [this](){ 
        buffer.lock()->GotoPreviousLine();
    })
    .AddAction("h", [this](){ 
        buffer.lock()->MoveCursorLeft();
    })
    .AddAction("l", [this](){ 
        buffer.lock()->MoveCursorRight();
    })
    .AddAction("i", [this](){ 
        nextState = Constants::InsertState;
    })
    .AddAction("o", [this](){ 
        nextState = Constants::InsertState;
        buffer.lock()->InsertLine();
        buffer.lock()->GotoNextLine();
    })
    .AddAction("u", [this](){ 
        undoHandler.UndoCommand();
    })
    .AddAction("r", [this](){ 
        undoHandler.RedoCommand();
    })
    .AddAction(":w", [this](){ 
        fileSaver.SaveToFile(buffer.lock());
    })
    .AddAction(":q", [this](){ 
        *quitToken = true;
    })
    .AddAction(":wq", [this](){ 
        *quitToken = true;
        fileSaver.SaveToFile(buffer.lock());
    })
    .AddAction("t", commandManager.CreateCommand<TestCommand>('t'));
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
        SwitchState(nextState);
    }
};
