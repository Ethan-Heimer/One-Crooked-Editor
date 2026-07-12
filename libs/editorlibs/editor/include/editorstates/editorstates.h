#pragma once

#include "ieditorstate.h"

#define EditorState(Name) \
    class Name : public EditorState{ \
        public: \
            using EditorState::EditorState; \
            constexpr string StateName() const override; \
            void OnUpdate() override; \
            void Transition() override; \
    }; \

namespace Editor::States{
    class EditorState : public IState{
        public:
            using IState::IState;
            string nextState;

    };

    EditorState(NormalState);
    EditorState(InsertState);
}
