#include "editorcommands/editorcommands.h"
#include "editorconstants.h"
#include "editorstates.h"

#include <ncurses.h>

using namespace Editor::States;

constexpr string NormalState::StateName() const{
    return Constants::NormalState;
}

void NormalState::OnUpdate(){
    nextState = StateName();
    if(inputQueue->empty())
        return;

    int input = inputQueue->front();    
    inputQueue->pop();

    if(input == ctrl('x'))
        *quitToken = true;
    else if(input == ctrl('w'))
        fileSaver.lock()->SaveToFile(buffer.lock());
    else if(input == ctrl('X')){
        *quitToken = true;
        fileSaver.lock()->SaveToFile(buffer.lock());
    }
    else if(input == KEY_DOWN || input == 'j'){
        buffer.lock()->GotoNextLine();
    } 
    else if(input == 'T'){
        commandManager.lock()->Execute<TestCommand>();
    } else if(input == 'U'){
        commandManager.lock()->Undo();
    }
    else if(input == 'R'){
        commandManager.lock()->Redo();
    }
    else if(input == KEY_UP || input == 'k'){
        buffer.lock()->GotoPreviousLine();
    }
    else if(input == KEY_LEFT || input == 'h'){
        buffer.lock()->MoveCursorLeft();
    }
    else if(input == KEY_RIGHT || input == 'l'){
        buffer.lock()->MoveCursorRight();
    } else if(input == 'i'){
        nextState = Constants::InsertState;
    }
}

void NormalState::Transition(){
    if(nextState != StateName()){
        stateMutator.lock()->ChangeState(nextState);
    }
};
