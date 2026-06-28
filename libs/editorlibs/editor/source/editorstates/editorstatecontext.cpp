#include "editorstatecontext.h"
#include "ieditorstate.h"
#include "statemachine.h"

#include <memory>

using namespace Editor::States;
using namespace StateMachines;
using namespace std;
using namespace Editor;

void StateContext::Initialize(const string& defaultState){

    std::shared_ptr<IEditorState> startingState = states[defaultState];

    stateMachine = std::make_unique<StateMachine<IEditorState>>(startingState);
}


void StateContext::Update(){
    stateMachine->Update();
}

void StateContext::ChangeState(const string state){
    stateMachine->SwitchState(states[state]);
}

std::weak_ptr<IStateMutator> StateContext::GetWeakPointer(){
    return weak_from_this();
}
