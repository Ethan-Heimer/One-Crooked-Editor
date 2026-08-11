#include <cctype>
#include <ncurses.h>

#include "editorcommands.h"
#include "editorstates.h"
#include "editorconstants.h"

using namespace Editor::States;
using namespace Editor::Actions;
using namespace CrookedEditor::States;
using namespace CrookedEditor::Mutators;

constexpr string InsertState::StateName() const{
    return Constants::InsertState;
}

void InsertState::OnEnter(){
    insertModeMutator = commandManager.RegesterMutation<InsertModeMutator>();
    actions
    .AddAction("\x1b", [this](){
        nextState = Constants::NormalState;
    })
    .AddAction("\b", [this](){
        if(cursor.IsCursorAtBeginningOfLine()){
            insertModeMutator->DoAction<DeleteLineAction>();
        }
        else
            insertModeMutator->DoAction<DeleteCharacterAction>();
    })
    .AddAction("\n", [this](){
        insertModeMutator->DoAction<NewLineAction>();
    })
    .AddAction("\r", [this](){
        insertModeMutator->DoAction<NewLineAction>();
    })
    .AddAction("\t", [this](){
        for(int i = 0; i < 4; i++){
            insertModeMutator->DoAction<InsertCharacterAction>(static_cast<char>(' '));
        }
    });
    
}

void InsertState::OnUpdate(){
    nextState = StateName();

    for(const char c : input){
        actions.TraverseToNextAction(c);

        if(std::isprint(c)){
            insertModeMutator->DoAction<InsertCharacterAction>(c);
        }
    }
}

void InsertState::OnExit(){
}

void InsertState::Transition(){
    if(nextState != StateName()){
        SwitchState(nextState);
    }
}
