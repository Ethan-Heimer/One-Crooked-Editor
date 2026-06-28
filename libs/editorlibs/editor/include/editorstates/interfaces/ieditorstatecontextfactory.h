#pragma once

#include "filehandling.h"
#include "ieditable.h"
#include "ieditorstatecontext.h"
#include <queue>

namespace Editor::States{
    class IStateContextFactory{
        public:
            virtual std::shared_ptr<IStateContext> Instanciate(std::weak_ptr<IFileSaver> fileSaver,
                    std::weak_ptr<IEditable> buffer, queue<int>* inputQueue, bool* quitToken) = 0;
    };
}
