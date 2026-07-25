#include "editorstatecontext.h"

#include "editorstates.h"
#include "ieditorstate.h"
#include "statemachine.h"

#include <memory>

using namespace Editor::States;
using namespace StateMachines;
using namespace std;
using namespace Editor;

void StateContext::Initialize(std::weak_ptr<IEditable> buffer, const string& defaultState, std::queue<int>* inputQueue, bool* quitToken){
    this->buffer = buffer;

    this->inputQueue = inputQueue;
    this->quitToken = quitToken;

    this->AddState<NormalState>();
    this->AddState<InsertState>();

    std::shared_ptr<IEditorState> startingState = states[defaultState];
    stateMachine = std::make_unique<StateMachine<IEditorState>>(startingState);
}

void StateContext::Update(){
    stateMachine->Update();
}

void StateContext::ChangeState(const std::string_view state){
    stateMachine->SwitchState(states[state.data()]);
}
