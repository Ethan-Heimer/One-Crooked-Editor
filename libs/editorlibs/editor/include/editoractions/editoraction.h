#pragma once

#include <memory>
namespace Editor::Actions{
    template<typename T>
    concept Is_Actionable_Function = requires (T action) {
        action();    
    };

    class Action final{
        public:
            template<typename T>
            Action(T action) : impl(std::make_unique<Model<T>>(std::move(action))){}

            void Invoke(){
                impl->Invoke();
            }

        private:
            struct Contract{
                virtual ~Contract() = default;
                
                virtual void Invoke() = 0;
            };

            template <typename T>
            requires Is_Actionable_Function<T>
            struct Model : Contract{
                T value;
                Model(T value) : value(std::move(value)){};

                void Invoke() override{
                    value();
                }
            };

            std::unique_ptr<Contract> impl;
    };
}
