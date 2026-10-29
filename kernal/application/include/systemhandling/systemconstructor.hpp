#pragma once

#include "configvault.hpp"
#include "eventbus.hpp"
#include <memory>
#include "isystem.hpp"

namespace CrookedEditor::Application::SystemHandling{
    template<SystemInterface T>
    class SystemType{
        public:
       
        template<typename... Args>
        std::shared_ptr<T> NewSystem(SystemID SID, Application& application,
                ConfigVault& configVault, Events::EventBus& eventBus, Args... args){
            return std::make_shared<T>(SID, application, configVault, eventBus, args...);
        }
    };

    template<typename T, typename... Args>
    concept SystemErasedType = requires (T x, SystemID SID, Application& application,
                ConfigVault& configVault, Events::EventBus& eventBus, Args... args) {
        x.NewSystem(SID, application, configVault, eventBus, args...);
    };

    class SystemConstructor{
        public:
            template <typename T, typename... Args> 
            requires SystemErasedType<T, Args...>
            SystemConstructor(T&& systemType, Args... args) 
            : impl(std::make_unique<Model<T, Args...>>(std::forward<T>(systemType), args...)){}

            std::shared_ptr<ISystem> ConstructSystem(SystemID SID, Application& application,
                ConfigVault& configVault, Events::EventBus& eventBus){
                return impl->ConstructSystem(SID, application, configVault, eventBus);
            }
        private:
            struct Contract{
                virtual std::shared_ptr<ISystem> ConstructSystem(SystemID SID, Application& application,
                ConfigVault& configVault, Events::EventBus& eventBus) = 0;
            };

            template<typename T, typename... Args>
            requires SystemErasedType<T, Args...>
            struct Model : public Contract{
                T systemType;
                std::tuple<Args...> arguments;

                Model(T&& systemType, Args... args) : systemType(systemType){
                    arguments = std::make_tuple(args...);
                }

                std::shared_ptr<ISystem> ConstructSystem(SystemID SID, Application& application,
                ConfigVault& configVault, Events::EventBus& eventBus){
                    return std::apply([&](Args... args){
                        return systemType.NewSystem(SID, application, configVault, eventBus, args...);
                    }, arguments);
                }
            };

            std::unique_ptr<Contract> impl;
    };
}

