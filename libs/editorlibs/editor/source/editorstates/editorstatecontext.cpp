#include "editorstatecontext.h"
#include "editorstateparameters.h"
#include "ieditable.h"
#include "statemachine.h"

#include <functional>
#include <map>
#include <string_view>

using namespace Editor::States;
using namespace StateMachines;
using namespace std;
using namespace Editor;
using namespace Mutators;

struct StateContext::Impl{
    public:
        Impl(StateParameters defaultStates, IEditable& buffer, function<void()> saveBuffer, 
                MutatorManager& commandManager, UndoHandler& undoHandler, bool* quitToken) 
            : buffer(buffer), saveBuffer(saveBuffer), commandManager(commandManager), 
            undoHandler(undoHandler), quitToken(quitToken){
                InitializeStates(defaultStates);
            }; 

        void AddState(StateTypeValue stateType, std::string* stateName){
            std::shared_ptr<IEditorState> newState 
                = stateType.Instanciate({buffer,
                        [this](string_view stateName){this->ChangeState(stateName);}, 
                        saveBuffer, commandManager, undoHandler, input, quitToken});

            std::string name = newState->StateName();
            states[newState->StateName()] = std::move(newState);

            if(stateName != nullptr)
                *stateName = name;
        }

        void Update(std::string_view input){
            this->input = input;
            stateMachine->Update();
        }
        
        void ChangeState(std::string_view state){
            stateMachine->SwitchState(states[state.data()]);
        }

        void InitializeStates(StateParameters& defaultStates){
            std::string defaultStateName{};
            defaultStates.AddDefaultStates(defaultStateName, [this](StateTypeValue stateType, string* stateName){
                this->AddState(std::move(stateType), stateName);
            });

            std::shared_ptr<IEditorState> startingState = states[defaultStateName];
            stateMachine = std::make_unique<StateMachine<IEditorState>>(startingState);
        }

        std::string CurrentStateName(){
            return stateMachine->CurrentState()->StateName();
        }

    private:
        map<string, shared_ptr<IEditorState>> states;
        unique_ptr<StateMachine<IEditorState>> stateMachine;

        IEditable& buffer;
        function<void()> saveBuffer;
        MutatorManager& commandManager;
        UndoHandler& undoHandler;
        std::string_view input{};
        bool* quitToken;
};

StateContext::StateContext(StateParameters startingStates, IEditable& buffer, 
        Mutators::MutatorManager& commandManager, std::function<void()> saveBuffer, 
        Mutators::UndoHandler& undoHandler, bool* quitToken) 

    : pImpl(std::make_unique<Impl>(std::move(startingStates), buffer, saveBuffer, commandManager, 
                undoHandler, quitToken)){}

StateContext::~StateContext() = default;

void StateContext::Update(std::string_view input){
    pImpl->Update(input);
}

void StateContext::ChangeState(const std::string_view state){
    pImpl->ChangeState(state);
}

std::string StateContext::CurrentStateName(){
    return pImpl->CurrentStateName();
}
