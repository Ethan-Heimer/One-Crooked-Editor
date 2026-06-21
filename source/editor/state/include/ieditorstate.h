#pragma once

#include <memory>

#include "buffer.h"
#include "gapbuffer/gapbuffer.h"
#include "linkedlist/doublyindexedlinkedlist.h"
#include "statemachine/istate.h"
#include "iinputmanager.h"
#include "context.h"

using namespace StateMachines;
using namespace Systems::Input;
using namespace std;

namespace Editor::States{
    class IEditorState : public IState{
        public:
            IEditorState(StateContext& context, shared_ptr<Editor> editor, 
                    shared_ptr<IInputManager> inputManager)
                : editor(editor), context(context), inputManager(inputManager){}

        protected:
            StateContext& context;
            shared_ptr<Editor> editor;

            shared_ptr<IInputManager> inputManager;
    };
}
