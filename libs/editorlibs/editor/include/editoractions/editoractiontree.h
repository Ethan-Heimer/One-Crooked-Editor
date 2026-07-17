#pragma once

#include "editoraction.h"
#include <string>
#include <memory>
#include <type_traits>

using namespace std;

namespace Editor::Actions{
    class ActionTree final{
        public:
            ActionTree();
            ~ActionTree();

            template<typename T>
            requires std::is_base_of_v<ActionBase, T>
            ActionTree& AddAction(string strokes, T action){
                AppendAction(strokes, make_unique<T>(action));
                return *this;
            }
            
            ActionTree& AppendAction(string strokes, unique_ptr<ActionBase> action);
            void TraverseToNextAction(char key);

        private:
            struct Impl;
            unique_ptr<Impl> pImpl;
    };
}
