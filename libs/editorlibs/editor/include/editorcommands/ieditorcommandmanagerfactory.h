#pragma once

#include "editorcommands/ieditorcommandmanager.h"
#include "ieditable.h"
#include "ieditorundohanderfactory.h"
#include <memory>
namespace Editor::Commands {
    class IEditorCommandManagerFactory{
        public:
            virtual std::shared_ptr<IEditorCommandManager> Instanciate
                (std::weak_ptr<IEditable> editable, std::weak_ptr<IEditorCommandUndoHandler> undoHandler) = 0;
    };
}
