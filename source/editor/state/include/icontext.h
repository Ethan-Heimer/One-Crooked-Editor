#pragma once


namespace Editor::States{
    enum States{
        Normal = 0,
        Insert = 1
    };

    class IStateContext{
        public:
        virtual void Update() = 0;
        virtual void ChangeState(States state) = 0;
    };
}
