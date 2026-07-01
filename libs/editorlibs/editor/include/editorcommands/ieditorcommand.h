#pragma once

#include "ieditable.h"
#include <memory>
namespace Editor::Commands {
    class IEditorCommand{
        public:
            IEditorCommand(std::weak_ptr<IEditable> buffer) : buffer(buffer){}

            virtual void Execute() = 0;
            virtual void Undo() = 0;

        protected:
            std::weak_ptr<IEditable> buffer;
    };
}
