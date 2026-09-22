#pragma once

#include <memory>
#include "editormutatortype.h"

namespace Editor::Mutators {
    class UndoHandler final{
        public: 
            UndoHandler();
            ~UndoHandler();

            void AddMutator(Mutator command);

            void UndoMutator();
            void RedoMutator(); 

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
