#pragma once

#include "ieditorcommandscontainer.h"

namespace Editor::Commands {
    class IEditorCommandUndoHandler : public IEditorCommandContainer{
        public:
            virtual void UndoCommand() = 0;
            virtual void RedoCommand() = 0; 
    };
}
