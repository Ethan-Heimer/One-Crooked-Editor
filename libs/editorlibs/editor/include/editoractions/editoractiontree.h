#pragma once

#include "editoraction.h"
#include <string>
#include <memory>

using namespace std;

namespace Editor::Actions{
    class ActionTree{
        public:
            template<typename T>
            requires Is_Actionable_Function<T>
            void AddAction(string strokes, T action){
                AddAction(strokes, make_unique<T>(action));
            }

            void AddAction(string strokes, unique_ptr<ActionBase> action);
            void DoAction(string strokes);

        private:
            struct Impl;
            unique_ptr<Impl> pImpl;
    };
}
