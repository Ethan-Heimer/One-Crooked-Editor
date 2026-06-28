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
    class EditorState : public IEditorState{
        public:
            using IEditorState::IEditorState;
            string nextState;

    };

    EditorState(NormalState);
    EditorState(InsertState);
}
