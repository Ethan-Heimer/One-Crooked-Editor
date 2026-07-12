#pragma once

#include "ieditorundohanderfactory.h"
#include "ieditorundohandler.h"
#include <memory>
#include <type_traits>

namespace Editor::Commands{
    template<typename T>
    requires std::is_base_of_v<IEditorCommandUndoHandler, T>
    class EditorUndoHandlerFactory : public IEditorUndoHandlerFactory{
        public:
            std::shared_ptr<IEditorCommandUndoHandler> Instanciate() override{
                std::shared_ptr<IEditorCommandUndoHandler> handler = std::make_shared<T>();
                return handler;
            };
    };
}
