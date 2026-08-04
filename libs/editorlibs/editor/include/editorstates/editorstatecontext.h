#pragma once

#include <memory>
#include <queue>
#include <string_view>

#include "ieditable.h"
#include "editormutations/editormutatormanager.h"
#include "editorstatetype.h"
#include "editorstateparameters.h"

namespace Editor::States{
    class StateContext final{
        public:
            StateContext(StateParameters states, IEditable& buffer, Mutators::MutatorManager& commandManager, 
                    std::function<void()> saveBuffer, Mutators::UndoHandler& undoHandler,
                    std::queue<int>* inputQueue, bool* quitToken);
            ~StateContext();

            void AddState(StateTypeValue stateType, std::string* stateName);
            void ChangeState(std::string_view name);

            void Start();
            void Update();

            private:
                struct Impl;
                std::unique_ptr<Impl> pImpl;
    };
}
