#pragma once

#include <memory>

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
            IEditorState(weak_ptr<IStateContext> context, weak_ptr<IEditor> editor, 
                    weak_ptr<IInputManager> inputManager)
                : editor(editor), context(context), inputManager(inputManager){}

        protected:
            weak_ptr<IStateContext> context;

            weak_ptr<IEditor> editor;
            weak_ptr<IInputManager> inputManager;
    };
}
