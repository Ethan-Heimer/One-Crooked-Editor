#pragma once

#include <memory>
#include <string_view>

#include "ieditable.h"
#include "editormutations/editormutatormanager.h"
#include "editorstatetype.h"
#include "editorstateparameters.h"

namespace Editor::States{
    class StateContext final{
        public:
            StateContext(StateParameters states, IEditable& buffer, Mutators::MutatorManager& commandManager, 
                    std::function<void()> saveBuffer, Mutators::UndoHandler& undoHandler, bool* quitToken);
            ~StateContext();

            void AddState(StateTypeValue stateType, std::string* stateName);
            void ChangeState(std::string_view name);

            void Start();
            void Update(std::string_view input);

            std::string CurrentStateName(); 

            private:
                struct Impl;
                std::unique_ptr<Impl> pImpl;
    };
}
