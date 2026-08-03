#include "editorcommands.h"
#include "editorconstants.h"
#include "editorstates.h"
#include "editoractions/editoraction.h"

#include <ncurses.h>

using namespace CrookedEditor::States;
using namespace CrookedEditor::Mutators;

constexpr string NormalState::StateName() const{
    return Constants::NormalState;
}

void NormalState::OnEnter(){
    actions
    .AddAction("j", [this](){ 
        cursor.lock()->GotoNextLine();
    })
    .AddAction("k", [this](){ 
        cursor.lock()->GotoPreviousLine();
    })
    .AddAction("h", [this](){ 
        cursor.lock()->MoveCursorLeft();
    })
    .AddAction("l", [this](){ 
        cursor.lock()->MoveCursorRight();
    })
    .AddAction("i", [this](){ 
        nextState = Constants::InsertState;
    })
    .AddAction("u", [this](){ 
        undoHandler.UndoMutator();
    })
    .AddAction("r", [this](){ 
        undoHandler.RedoMutator();
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
    });
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
