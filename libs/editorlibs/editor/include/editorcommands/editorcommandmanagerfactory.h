#pragma once

#include "editorcommands/ieditorcommandmanager.h"
#include "editorcommands/ieditorcommandmanagerfactory.h"
#include "ieditable.h"
#include <memory>
#include <type_traits>
namespace Editor::Commands{
    template <typename T>
    requires std::is_base_of_v<ICommandManager, T>
    class CommandManagerFactory : public ICommandManagerFactory{
        public:
            std::shared_ptr<ICommandManager> 
                Instanciate(std::weak_ptr<IEditable> buffer, weak_ptr<IUndoHandler> undoHandler) override{
                return std::make_shared<T>(buffer, undoHandler);
            };
    };
}
