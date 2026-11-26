#include "istate.h"
#include "statemachine.h"
#include <cwchar>
#include <iostream>

using namespace StateMachines;

class StateContext;

class StateOne : public IState{
    public:
        StateOne(StateContext& context) : context(context){}

        void OnUpdate() override;
        void Transition() override;

    private:
        StateContext& context;
};

class StateTwo : public IState{
    public:
        StateTwo(StateContext& context) : context(context){}

        void OnUpdate() override;
        void Transition() override;

    private:
        StateContext& context;
};

class StateContext{
    public:
        StateContext();

        std::shared_ptr<StateOne> stateOne;
        std::shared_ptr<StateTwo> stateTwo;
        
        int count = 0;

        void SwitchState(std::shared_ptr<IState> state);
        void Update();

    private:
        StateMachine<IState> stateMachine;
};

void StateOne::OnUpdate(){
    std::cout << "State One!" << std::endl;
}

void StateOne::Transition(){
    if(context.count > 11){
       context.SwitchState(context.stateTwo); 
        std::cout << "transition one" << std::endl;
    }
}

void StateTwo::OnUpdate(){
    std::cout << "State Two!" << std::endl;
}

void StateTwo::Transition(){

    if(context.count % 13 == 0){
       context.SwitchState(context.stateOne); 
       std::cout << "transition" << std::endl;
    }
}

StateContext::StateContext() : stateOne(std::make_shared<StateOne>(*this)), 
    stateTwo(std::make_shared<StateTwo>(*this)), stateMachine(stateOne){};

void StateContext::SwitchState(std::shared_ptr<IState> state){
    stateMachine.SwitchState(state);
}

void StateContext::Update(){
    stateMachine.Update();
}

int main(){
    StateContext stateContext; 

    while(true){
        stateContext.Update();
        stateContext.count++;
    }
}
