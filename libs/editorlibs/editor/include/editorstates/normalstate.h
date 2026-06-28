#pragma once

#include "ieditorstate.h"
namespace Editor::States{
    class NormalState : public IEditorState{
        public:
        using IEditorState::IEditorState;
        string nextState;

        constexpr string StateName() const override;
        void OnUpdate() override;
        void Transition() override;
    };
}
