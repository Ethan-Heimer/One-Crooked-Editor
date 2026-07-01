#pragma once

#include "editorcommands/ieditorcommandmanager.h"
#include "ieditable.h"
#include <memory>
namespace Editor::Commands {
    class IEditorCommandManagerFactory{
        public:
            virtual std::shared_ptr<IEditorCommandManager> Instanciate
                (std::weak_ptr<IEditable> editable) = 0;
    };
}
