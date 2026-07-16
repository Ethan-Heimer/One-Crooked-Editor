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

using namespace Editor::Actions;

namespace Editor::States{
    class EditorState : public IState{
        public:
            using IState::IState;

            string nextState{};
            ActionTree actions{};
    };

    EditorState(NormalState);
    EditorState(InsertState);
}
