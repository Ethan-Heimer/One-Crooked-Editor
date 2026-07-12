#pragma once

#include "editorcommands/ieditorcommandmanager.h"
#include "editorcommands/ieditorcommandmanagerfactory.h"
#include "ieditable.h"
#include <memory>
#include <type_traits>
namespace Editor::Commands{
    template <typename T>
    requires std::is_base_of_v<IEditorCommandManager, T>
    class EditorCommandManagerFactory : public IEditorCommandManagerFactory{
        public:
            std::shared_ptr<IEditorCommandManager> 
                Instanciate(std::weak_ptr<IEditable> buffer, weak_ptr<IEditorCommandUndoHandler> undoHandler) override{
                return std::make_shared<T>(buffer, undoHandler);
            };
    };
}
