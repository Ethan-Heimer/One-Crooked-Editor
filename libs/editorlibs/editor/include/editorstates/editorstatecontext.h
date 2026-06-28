#pragma once
#include "ieditorstatecontext.h"

#include <memory>
#include "ieditorstate.h"
#include "statemachine.h"

using namespace std;

namespace Editor::States{
    class StateContext : public IStateContext{
        public:
            using IStateContext::IStateContext;

            void Initialize(const string& defaultState) override;

            void Update() override;
            void ChangeState(const string state) override;            

            std::weak_ptr<IStateMutator> GetWeakPointer() override;

        private:
            std::unique_ptr<StateMachines::StateMachine<IEditorState>> stateMachine;
    };
}
