#pragma once

#include "editorcommands/ieditorcommandmanager.h"
#include "ieditable.h"
#include "ieditorundohanderfactory.h"
#include <memory>

namespace Editor::Commands {
    class ICommandManagerFactory{
        public:
            virtual std::shared_ptr<ICommandManager> Instanciate
                (std::weak_ptr<IEditable> editable, std::weak_ptr<IUndoHandler> undoHandler) = 0;
    };
}
