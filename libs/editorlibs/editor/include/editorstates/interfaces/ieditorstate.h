#pragma once

#include <memory>
#include <queue>

#include "editorfilehandling/ieditorfilehandler.h"
#include "filehandling.h"
#include "ieditable.h"
#include "istate.h"
#include "ieditorstatemutator.h"

using namespace StateMachines;
using namespace std;

namespace Editor::States{
    class IEditorState : public IState{
        public:
            IEditorState(weak_ptr<IFileSaver> fileSaver, 
                    weak_ptr<IEditable> buffer, weak_ptr<IStateMutator> stateMutator, queue<int>* inputQueue, bool* quitToken)
                : stateMutator(stateMutator), inputQueue(inputQueue), fileSaver(fileSaver), buffer(buffer), quitToken(quitToken){}

            virtual constexpr string StateName() const = 0;

        protected:
            weak_ptr<IFileSaver> fileSaver;
            weak_ptr<IEditable> buffer;
            weak_ptr<IStateMutator> stateMutator;

            queue<int>* inputQueue;
            bool* quitToken;
    };
}
