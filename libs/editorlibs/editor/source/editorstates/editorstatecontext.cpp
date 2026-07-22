#include "editorstatecontext.h"
#include "editorstates.h"
#include "ieditorstate.h"
#include "statemachine.h"

#include <memory>

using namespace Editor::States;
using namespace StateMachines;
using namespace std;
using namespace Editor;

void StateContext::Initialize(std::weak_ptr<IEditable> buffer, std::weak_ptr<FileHandling::IFileHandler> fileSaver, const string& defaultState, std::queue<int>* inputQueue, bool* quitToken){
    this->buffer = buffer;
    this->fileSaver = fileSaver;

    this->inputQueue = inputQueue;
    this->quitToken = quitToken;

    this->AddState<NormalState>();
    this->AddState<InsertState>();

    std::shared_ptr<IState> startingState = states[defaultState];
    stateMachine = std::make_unique<StateMachine<IState>>(startingState);
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
