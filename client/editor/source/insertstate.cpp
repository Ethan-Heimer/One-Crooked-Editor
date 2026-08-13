#include <cctype>
#include <ncurses.h>

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
    insertModeMutator = mutationManager.CreateMutation<InsertModeMutator>();
    actions
    .AddAction("\x1b", [this](){
        nextState = Constants::NormalState;
    })
    .AddAction({127}, [this](){
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
    .AddAction({10}, [this](){ 
        cursor.GotoNextLine();
    })
    .AddAction({11}, [this](){ 
        cursor.GotoPreviousLine();
    })
    .AddAction({8}, [this](){ 
        cursor.MoveCursorLeft();
    })
    .AddAction({12}, [this](){ 
        cursor.MoveCursorRight();
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
