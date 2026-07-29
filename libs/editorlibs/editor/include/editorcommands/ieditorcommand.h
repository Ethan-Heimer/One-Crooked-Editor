#pragma once

#include "ieditable.h"
#include "editorcommandtype.h"
#include <memory>

namespace Editor::Commands {
    class ICommandBehavior {
        public:
            ICommandBehavior(std::weak_ptr<IEditable> buffer) : buffer(buffer){}
            ICommandBehavior(const ICommandBehavior& other){
                buffer = other.buffer;
            }
            ICommandBehavior(ICommandBehavior&& other){
                buffer = other.buffer;
                other.buffer.reset();
            }

            virtual ~ICommandBehavior() = default;
 
            virtual void Initialize() = 0;
            virtual void Do() = 0;
            virtual void Undo() = 0;

        protected:
            std::weak_ptr<IEditable> buffer;

    };
}
