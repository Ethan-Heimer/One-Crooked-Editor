#include "context.h"
#include "buffer.h"
#include "icontext.h"
#include "ieditorstate.h"
#include "iinputmanager.h"
#include "insertstate.h"
#include "normalstate.h"
#include "statemachine/statemachine.h"

#include <memory>
#include <vector>

using namespace Editor::States;
using namespace StateMachines;
using namespace std;
using namespace Editor;

void StateContext::Initialize(weak_ptr<Buffers::IBufferFileHandler> fileHandler, 
    weak_ptr<Buffers::Buffer> buffer, weak_ptr<Systems::Input::IInputManager> inputManager, bool* quitToken){

    normalState = std::make_shared<NormalState>(GetWeakPointer(), fileHandler, buffer, inputManager, quitToken);
    insertState = std::make_shared<InsertState>(GetWeakPointer(), fileHandler, buffer, inputManager,  quitToken);

    stateMachine = std::make_unique<StateMachine<IEditorState>>(normalState);

    states.push_back(normalState);
    states.push_back(insertState);
}


void StateContext::Update(){
    stateMachine->Update();
}

void StateContext::ChangeState(States state){
    stateMachine->SwitchState(states[state]);
}

std::weak_ptr<IStateContext> StateContext::GetWeakPointer(){
    return weak_from_this();
}
