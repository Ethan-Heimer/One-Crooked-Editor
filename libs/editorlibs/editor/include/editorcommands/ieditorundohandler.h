#pragma once

#include "ieditorcommandscontainer.h"

namespace Editor::Commands {
    class IUndoHandler : public ICommandContainer{
        public:
            virtual void UndoCommand() = 0;
            virtual void RedoCommand() = 0; 
    };
}
