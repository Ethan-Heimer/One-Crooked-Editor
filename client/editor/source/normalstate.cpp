#include "editorconstants.h"
#include "editorstates.h"
#include "editoractions/editoraction.h"
#include "newlinemutator.hpp"

#include <ncurses.h>

using namespace CrookedEditor::States;
using namespace CrookedEditor::Mutators;

constexpr string NormalState::StateName() const{
    return Constants::NormalState;
}

void NormalState::OnEnter(){
    constexpr int jumpBy = 10;
    actions
    .AddAction("j", [this](){ 
        cursor.GotoNextLine();
    })
    .AddAction("k", [this](){ 
        cursor.GotoPreviousLine();
    })
    .AddAction("J", [this](){ 
        for(int i = 0; i < jumpBy; i++)
            cursor.GotoNextLine();
    })
    .AddAction("K", [this](){ 
        for(int i = 0; i < jumpBy; i++)
            cursor.GotoPreviousLine();
    })
    .AddAction("h", [this](){ 
        cursor.MoveCursorLeft();
    })
    .AddAction("l", [this](){ 
        cursor.MoveCursorRight();
    })
    .AddAction("i", [this](){ 
        nextState = Constants::InsertState;
    })
    .AddAction("o", [this](){ 
        mutationManager.DoMutation<NewLineMutator>();
        nextState = Constants::InsertState;
    })
    .AddAction("u", [this](){ 
        undoHandler.UndoMutator();
    })
    .AddAction("r", [this](){ 
        undoHandler.RedoMutator();
    })
    .AddAction(":w", [this](){ 
        SaveBuffer();
    })
    .AddAction(":q", [this](){ 
        *quitToken = true;
    })
    .AddAction(":wq", [this](){ 
        *quitToken = true;
        SaveBuffer();
    });
}

void NormalState::OnUpdate(){
    nextState = StateName();

    for(const char c : input){
        actions.TraverseToNextAction(c);
    }
}

void NormalState::Transition(){
    if(nextState != StateName()){
        SwitchState(nextState);
    }
};
