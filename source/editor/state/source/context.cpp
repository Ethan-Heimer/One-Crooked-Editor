#include "context.h"
#include "buffer.h"
#include "gapbuffer/gapbuffer.h"
#include "ieditorstate.h"
#include "iinputmanager.h"
#include "insertstate.h"
#include "linkedlist/doublyindexedlinkedlist.h"
#include "normalstate.h"
#include "statemachine/statemachine.h"

#include <iostream>
#include <memory>
#include <vector>

using namespace Editor::States;
using namespace StateMachines;
using namespace std;

void StateContext::Initialize(weak_ptr<IEditor> editor, weak_ptr<IInputManager> inputManager){    
    normalState = std::make_shared<NormalState>(GetWeakPointer(), editor, inputManager);
    insertState = std::make_shared<InsertState>(GetWeakPointer(), editor, inputManager);

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
