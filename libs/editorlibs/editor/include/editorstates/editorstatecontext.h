#pragma once

#include <memory>
#include <queue>
#include <string_view>

#include "editorfilehandler.h"
#include "ieditable.h"
#include "editormutations/editormutatormanager.h"
#include "editorstatetype.h"
#include "editorstateparameters.h"

namespace Editor::States{
    class StateContext final{
        public:
            StateContext(Mutators::MutatorManager& commandManager, 
                    FileHandling::FileHandler& fileHandler, Mutators::UndoHandler& undoHandler);
            ~StateContext();

            void AddState(StateTypeValue stateType, std::string* stateName);
            void ChangeState(std::string_view name);

            void Initialize(std::weak_ptr<IEditable> buffer, StateParameters states, 
                    std::queue<int>* inputQueue, bool* quitToken);

            void Start();
            void Update();

            private:
                struct Impl;
                std::unique_ptr<Impl> pImpl;
    };
}
