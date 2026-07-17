#pragma once

#include "ieditorundohandler.h"
#include <memory>

namespace Editor::Commands{
    class IUndoHandlerFactory{
        public:
            virtual std::shared_ptr<IUndoHandler> Instanciate() = 0;
    };
}
