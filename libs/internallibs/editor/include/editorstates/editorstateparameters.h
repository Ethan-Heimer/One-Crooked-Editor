#pragma once

#include "editorstatetype.h"

namespace Editor::States{
    template <typename D, typename... S>
    class DefaultStates{
        public:
            void AddDefaultStates(std::string& defaultStateName, 
                    std::function<void(StateTypeValue stateType, std::string* stateName)> addState){
                addState(StateType<D>{}, &defaultStateName);
                (addState(StateType<S>{}, nullptr), ...);
            }
    };

    class StateParameters{
        public:
            template <typename T>
            StateParameters(T params) 
            : impl(std::make_unique<Model<T>>(std::move(params))){}

            void AddDefaultStates(std::string& defaultStateName, 
                std::function<void(StateTypeValue stateType, std::string* stateName)> addState){
                impl->AddDefaultStates(defaultStateName, addState);
            }

        private:
            struct Contract{
                virtual ~Contract() = default;
                virtual void AddDefaultStates(std::string& defaultStateName,
                    std::function<void(StateTypeValue stateType, std::string* stateName)> addState) = 0;
            };

            template <typename T>
            struct Model : public Contract{
                T params; 
                Model(T&& params) : params(std::move(params)){}

                void AddDefaultStates(std::string& defaultStateName,
                     std::function<void(StateTypeValue stateType, std::string* stateName)> addState){
                    params.AddDefaultStates(defaultStateName, addState);
                }
            };

            std::unique_ptr<Contract> impl;
    };

}
