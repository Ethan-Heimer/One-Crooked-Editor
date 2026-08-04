#include "editorstatecontext.h"
#include "editorstateparameters.h"
#include "ieditable.h"
#include "statemachine.h"

#include <map>

using namespace Editor::States;
using namespace StateMachines;
using namespace std;
using namespace Editor;
using namespace Mutators;
using namespace FileHandling;

struct StateContext::Impl{
    public:
        Impl(StateParameters defaultStates, IEditable& buffer, FileHandler& fileHandler, 
                MutatorManager& commandManager, UndoHandler& undoHandler, std::queue<int>* inputQueue, bool* quitToken) 
            : buffer(buffer), fileHandler(fileHandler), commandManager(commandManager), 
            undoHandler(undoHandler), inputQueue(inputQueue), quitToken(quitToken){
                std::string defaultStateName{};
                defaultStates.AddDefaultStates(defaultStateName, [this](StateTypeValue stateType, string* stateName){
                    this->AddState(std::move(stateType), stateName);
                });

                std::shared_ptr<IEditorState> startingState = states[defaultStateName];
                stateMachine = std::make_unique<StateMachine<IEditorState>>(startingState);
            }; 

        void AddState(StateTypeValue stateType, std::string* stateName){
            std::shared_ptr<IEditorState> newState 
                = stateType.Instanciate({fileHandler, buffer, 
                        [this](string_view stateName){this->ChangeState(stateName);}, 
                        commandManager, undoHandler, inputQueue, quitToken});

            std::string name = newState->StateName();
            states[newState->StateName()] = std::move(newState);

            if(stateName != nullptr)
                *stateName = name;
        }

        void Update(){
            stateMachine->Update();
        }
        
        void ChangeState(std::string_view state){
            stateMachine->SwitchState(states[state.data()]);
        }

    private:
        map<string, shared_ptr<IEditorState>> states;
        unique_ptr<StateMachine<IEditorState>> stateMachine;

        FileHandler& fileHandler;
        MutatorManager& commandManager;
        UndoHandler& undoHandler;
        IEditable& buffer;
        queue<int>* inputQueue;
        bool* quitToken;
};

StateContext::StateContext(StateParameters startingStates, IEditable& buffer, 
        Mutators::MutatorManager& commandManager, FileHandling::FileHandler& fileHandler, 
        Mutators::UndoHandler& undoHandler, std::queue<int>* inputQueue, bool* quitToken) 

    : pImpl(std::make_unique<Impl>(std::move(startingStates), buffer, fileHandler, commandManager, 
                undoHandler, inputQueue, quitToken)){}

StateContext::~StateContext() = default;

void StateContext::Update(){
    pImpl->Update();
}

void StateContext::ChangeState(const std::string_view state){
    pImpl->ChangeState(state);
}
