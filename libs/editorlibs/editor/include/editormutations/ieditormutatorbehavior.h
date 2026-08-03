#pragma once

#include "ieditable.h"
#include <memory>

namespace Editor::Mutators {
    class IMutatorBehavior {
        public:
            IMutatorBehavior(std::weak_ptr<IEditable> buffer) : buffer(buffer){}
            IMutatorBehavior(const IMutatorBehavior& other){
                buffer = other.buffer;
            }
            IMutatorBehavior(IMutatorBehavior&& other){
                buffer = other.buffer;
                other.buffer.reset();
            }

            virtual ~IMutatorBehavior() = default;
 
            virtual void Do() = 0;
            virtual void Undo() = 0;

        protected:
            std::weak_ptr<IEditable> buffer;

    };
}
