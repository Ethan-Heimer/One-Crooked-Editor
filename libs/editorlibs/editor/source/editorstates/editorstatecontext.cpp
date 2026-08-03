#include "editorstatecontext.h"
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
        Impl(FileHandler& fileHandler, MutatorManager& commandManager, UndoHandler& undoHandler) 
            : fileHandler(fileHandler), commandManager(commandManager), undoHandler(undoHandler){}; 

        void Initialize(std::weak_ptr<IEditable> buffer, StateParameters defaultStates,
            std::queue<int>* inputQueue, bool* quitToken){
            this->buffer = buffer;

            this->inputQueue = inputQueue;
            this->quitToken = quitToken;
    
            std::string defaultStateName{};
            defaultStates.AddDefaultStates(defaultStateName, [this](StateTypeValue stateType, string* stateName){
                this->AddState(std::move(stateType), stateName);
            });

            std::shared_ptr<IEditorState> startingState = states[defaultStateName];
            stateMachine = std::make_unique<StateMachine<IEditorState>>(startingState);
        }

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
        weak_ptr<IEditable> buffer;
        queue<int>* inputQueue;
        bool* quitToken;
};

StateContext::StateContext(Mutators::MutatorManager& commandManager, FileHandling::FileHandler& fileHandler, Mutators::UndoHandler& undoHandler) 
    : pImpl(std::make_unique<Impl>(fileHandler, commandManager, undoHandler)){}

StateContext::~StateContext() = default;

void StateContext::Initialize(std::weak_ptr<IEditable> buffer, StateParameters defaultStates,
        std::queue<int>* inputQueue, bool* quitToken){
    pImpl->Initialize(buffer, std::move(defaultStates), inputQueue, quitToken);
}

void StateContext::Update(){
    pImpl->Update();
}

void StateContext::ChangeState(const std::string_view state){
    pImpl->ChangeState(state);
}
