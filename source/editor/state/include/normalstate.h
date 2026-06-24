#pragma once

#include "ieditorstate.h"
namespace Editor::States{
    class NormalState : public IEditorState{
        public:
        using IEditorState::IEditorState;
        States nextState;

        void OnUpdate() override;
        void Transition() override;
    };
}
