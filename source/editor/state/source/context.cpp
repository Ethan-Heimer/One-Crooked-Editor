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

struct StateContext::Impl{
    vector<shared_ptr<IEditorState>> states;

    shared_ptr<NormalState> normalState;
    shared_ptr<InsertState> insertState;

    unique_ptr<StateMachine<IEditorState>> stateMachine;

    Impl(StateContext& context, shared_ptr<Editor> editor, 
            shared_ptr<IInputManager> inputManager){
       normalState = std::make_shared<NormalState>(context, editor, inputManager);
       insertState = std::make_shared<InsertState>(context, editor, inputManager);

       stateMachine = std::make_unique<StateMachine<IEditorState>>(normalState);

       states.push_back(normalState);
       states.push_back(insertState);
    }

    void Update(){
        stateMachine->Update();
    }

    void ChangeState(States state){
       stateMachine->SwitchState(states[state]);
    }
};

StateContext::StateContext(shared_ptr<Editor> editor, shared_ptr<IInputManager> inputManager)
    : pImpl(std::make_unique<Impl>(*this, editor, inputManager)){}


StateContext::~StateContext() = default;


void StateContext::Update(){
    pImpl->Update();
}

void StateContext::ChangeState(States state){
    pImpl->ChangeState(state);
}
