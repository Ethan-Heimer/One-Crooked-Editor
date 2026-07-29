#pragma once

#include "editoractiontree.h"
#include "ieditorstate.h"

#define EditorState(Name) \
    class Name : public EditorState{ \
        public: \
            using EditorState::EditorState; \
            constexpr string StateName() const override; \
            void OnUpdate() override; \
            void Transition() override; \
            void OnEnter() override; \
    }; \

namespace CrookedEditor::States{
    class EditorState : public Editor::States::IEditorState{
        public:
            using IEditorState::IEditorState;

            string nextState{};
            Editor::Actions::ActionTree actions{};
    };

    EditorState(NormalState);
    EditorState(InsertState);
}
