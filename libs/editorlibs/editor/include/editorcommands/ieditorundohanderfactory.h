#pragma once

#include "ieditorundohandler.h"
#include <memory>

namespace Editor::Commands{
    class IEditorUndoHandlerFactory{
        public:
            virtual std::shared_ptr<IEditorCommandUndoHandler> Instanciate() = 0;
    };
}
