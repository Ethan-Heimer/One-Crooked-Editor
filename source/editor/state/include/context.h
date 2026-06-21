#pragma once

#include <memory>
#include <iinputmanager.h>

#include "icontext.h"
#include "ieditor.h"
#include "ieditorstate.h"
#include "statemachine/statemachine.h"

#include "normalstate.h"
#include "insertstate.h"

using namespace std;
using namespace Systems;

namespace Editor::States{
    class StateContext : public IStateContext, public std::enable_shared_from_this<IStateContext>{
        public:
            void Initialize(weak_ptr<IEditor> editor, weak_ptr<Input::IInputManager> inputManager);

            void Update() override;
            void ChangeState(States state) override;

            std::weak_ptr<IStateContext> GetWeakPointer();            
            

        private:
            std::unique_ptr<StateMachines::StateMachine<IEditorState>> stateMachine;
            shared_ptr<NormalState> normalState;
            shared_ptr<InsertState> insertState;

            vector<shared_ptr<IEditorState>> states;
    };
}
