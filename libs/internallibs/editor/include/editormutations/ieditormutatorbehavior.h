#pragma once

#include "ieditable.h"

namespace Editor::Mutators {
    class IMutatorBehavior {
        public:
            IMutatorBehavior(IEditable& buffer) : buffer(buffer){}
            IMutatorBehavior(const IMutatorBehavior& other) : buffer(other.buffer){}
            IMutatorBehavior(IMutatorBehavior&& other) : buffer(other.buffer){}

            virtual ~IMutatorBehavior() = default;
 
            virtual void Do() = 0;
            virtual void Undo() = 0;

        protected:
            IEditable& buffer;

    };
}
