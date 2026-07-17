#pragma once

#include "ieditable.h"
#include <algorithm>
#include <memory>
#include "ieditorcommandscontainer.h"

namespace Editor::Commands {
    class ICommand {
        public:
            ICommand(std::weak_ptr<IEditable> buffer, 
                    std::weak_ptr<ICommandContainer> undoHandler) : buffer(buffer), undoHandler(undoHandler){}
            virtual ~ICommand() = default;
 
            virtual void Do() = 0;
            virtual void Undo() = 0;

            void operator() (){
                Execute();   
            };

        protected:
            std::weak_ptr<ICommandContainer> undoHandler;
            std::weak_ptr<IEditable> buffer;

            virtual void Initialize() = 0;
            virtual void Execute() = 0;

            virtual std::unique_ptr<ICommand> Clone() = 0;
    };
}
