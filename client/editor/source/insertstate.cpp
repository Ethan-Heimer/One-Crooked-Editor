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
            insertModeMutator->DoAction<InsertModeMutator::DeleteLineAction>();
        }
        else
            insertModeMutator->DoAction<InsertModeMutator::DeleteCharacterAction>();
    })
    .AddAction("\n", [this](){
        insertModeMutator->DoAction<InsertModeMutator::NewLineAction>();
    })
    .AddAction("\r", [this](){
        insertModeMutator->DoAction<InsertModeMutator::NewLineAction>();
    });
    /*
    .AddAction("\t", [this](){
        for(int i = 0; i < 4; i++){
            buffer.lock()->InsertCharacter(' ');
        }
    });
    */
    
}

void InsertState::OnUpdate(){
    nextState = StateName();

    if(inputQueue->empty())
        return;

    int input = inputQueue->front();
    inputQueue->pop();

    actions.TraverseToNextAction(static_cast<char>(input));

    if(std::isprint(static_cast<unsigned char>(input))){
        insertModeMutator->DoAction<InsertModeMutator::InsertCharacterAction>(static_cast<char>(input));
    }
}

void InsertState::OnExit(){
}

void InsertState::Transition(){
    if(nextState != StateName()){
        SwitchState(nextState);
    }
}
