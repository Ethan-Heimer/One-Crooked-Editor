#pragma once

namespace Editor::Actions{
    template<typename T>
    concept Is_Actionable_Function = requires (T action) {
        action();    
    };

    struct ActionBase{
        virtual void Invoke() = 0;
    };

    template<Is_Actionable_Function Func>
    struct Action final : ActionBase{
        Func action; 

        explicit Action(Func action) : action(action){}

        void Invoke() override{
            action();
        };
    };
}
