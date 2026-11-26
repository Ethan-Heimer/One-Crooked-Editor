#pragma once

#include "ieditormutatorbehavior.h"
namespace CrookedEditor::Mutators {
    class NewLineMutator : public Editor::Mutators::IMutatorBehavior{
        public:
            unsigned int lineNumber;
            NewLineMutator(Editor::IEditable& editable);
            NewLineMutator(const NewLineMutator& other);
            NewLineMutator(NewLineMutator&& other);
 
            void Do() override;
            void Undo() override;
    };
}
