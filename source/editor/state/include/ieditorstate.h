#pragma once

#include <memory>

#include "buffer.h"
#include "ibufferfilehandler.h"
#include "icontext.h"
#include "statemachine/istate.h"
#include "iinputmanager.h"
#include "icontext.h"
#include "ieditor.h"

using namespace StateMachines;
using namespace Systems::Input;
using namespace std;

namespace Editor::States{
    class IEditorState : public IState{
        public:
            IEditorState(weak_ptr<IStateContext> context, weak_ptr<Buffers::IBufferFileHandler> fileHandler, 
                    weak_ptr<Buffers::Buffer> buffer, weak_ptr<IInputManager> inputManager, bool* quitToken)
                : context(context), inputManager(inputManager), fileHandler(fileHandler), buffer(buffer), quitToken(quitToken){}

        protected:
            weak_ptr<IStateContext> context;
            weak_ptr<IInputManager> inputManager;
            weak_ptr<Buffers::IBufferFileHandler> fileHandler;
            weak_ptr<Buffers::Buffer> buffer;
            bool* quitToken;
    };
}
