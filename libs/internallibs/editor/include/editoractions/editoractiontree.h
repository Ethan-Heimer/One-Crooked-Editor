#pragma once

#include "editoraction.h"
#include <string>
#include <memory>

using namespace std;

namespace Editor::Actions{
    class ActionTree final{
        public:
            ActionTree();
            ~ActionTree();

            ActionTree& AddAction(string strokes, Action action);
            void TraverseToNextAction(char key);

        private:
            struct Impl;
            unique_ptr<Impl> pImpl;
    };
}
