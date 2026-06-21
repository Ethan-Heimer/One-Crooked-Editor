#pragma once

#include "gapbuffer/gapbuffer.h"
#include "linkedlist/doublyindexedlinkedlist.h"
#include <memory>
#include <iinputmanager.h>
#include "editor.h"


using namespace std;
using namespace Systems;

namespace Editor::States{
    enum States{
        Normal = 0,
        Insert = 1
    };

    class StateContext{
        public:
            StateContext(shared_ptr<Editor> editor, shared_ptr<Input::IInputManager> inputManager);
            ~StateContext();

            void Update();
            void ChangeState(States state);
            
        private:
            struct Impl; 
            std::unique_ptr<Impl> pImpl;
    };
}
