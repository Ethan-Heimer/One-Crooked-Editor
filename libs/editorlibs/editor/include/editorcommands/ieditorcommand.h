#pragma once

#include "ieditable.h"
#include <algorithm>
#include <memory>
#include "ieditorcommandscontainer.h"

namespace Editor::Commands {
    class IEditorCommand {
        public:
            IEditorCommand(std::weak_ptr<IEditable> buffer, 
                    std::weak_ptr<IEditorCommandContainer> undoHandler) : buffer(buffer), undoHandler(undoHandler){}
            virtual ~IEditorCommand() = default;
 
            virtual void Execute() = 0;

            virtual void Do() = 0;
            virtual void Undo() = 0;

            virtual std::unique_ptr<IEditorCommand> Clone() = 0;

            void operator() (){
                Execute();   
            };

        protected:
            std::weak_ptr<IEditorCommandContainer> undoHandler;
            std::weak_ptr<IEditable> buffer;
    };
}
