#pragma once

#include <memory>
#include <queue>

#include "buffer.h"
#include "ibufferfilehandler.h"
#include "istate.h"
#include "ieditorstatemutator.h"

#define ctrl(x) ((x) & 0x1f)

using namespace StateMachines;
using namespace std;

namespace Editor::States{
    class IStateContext;

    class IEditorState : public IState{
        public:
            IEditorState(weak_ptr<Buffers::IBufferFileHandler> fileHandler, 
                    weak_ptr<Buffers::Buffer> buffer, weak_ptr<IStateMutator> stateMutator, queue<int>* inputQueue, bool* quitToken)
                : stateMutator(stateMutator), inputQueue(inputQueue), fileHandler(fileHandler), buffer(buffer), quitToken(quitToken){}

            virtual constexpr string StateName() const = 0;

        protected:
            weak_ptr<Buffers::IBufferFileHandler> fileHandler;
            weak_ptr<Buffers::Buffer> buffer;
            weak_ptr<IStateMutator> stateMutator;

            queue<int>* inputQueue;
            bool* quitToken;
    };
}
