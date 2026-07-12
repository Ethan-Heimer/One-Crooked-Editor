#pragma once

#include "ieditorundohanderfactory.h"
#include "ieditorundohandler.h"
#include <memory>
#include <type_traits>

namespace Editor::Commands{
    template<typename T>
    requires std::is_base_of_v<IUndoHandler, T>
    class UndoHandlerFactory : public IUndoHandlerFactory{
        public:
            std::shared_ptr<IUndoHandler> Instanciate() override{
                std::shared_ptr<IUndoHandler> handler = std::make_shared<T>();
                return handler;
            };
    };
}
