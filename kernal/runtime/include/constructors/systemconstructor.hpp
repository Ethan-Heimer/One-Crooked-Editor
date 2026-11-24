#pragma once

#include "isystem.hpp"
#include <cstddef>
#include <memory>
#include "systemtypes.hpp"

namespace CrookedEditor::Runtime{
    template<typename T>
    class SystemType{
        public:
       
        std::shared_ptr<T> NewSystem(API::SystemID SID, Events::RequestBus& requestBus){
            return std::make_shared<T>(SID, requestBus);
        }
    };

    template<typename T>
    concept SystemErasedType = requires (T x, API::SystemID SID, Events::RequestBus& requestBus) {
        x.NewSystem(SID, requestBus);
    };

    class SystemConstructor{
        public:
            template <typename T> 
            requires SystemErasedType<T>
            SystemConstructor(T&& systemType) 
            : impl(std::make_unique<Model<T>>(std::forward<T>(systemType))){}

            std::shared_ptr<API::ISystem> ConstructSystem(API::SystemID SID, Events::RequestBus& requestBus){
                return impl->ConstructSystem(SID, requestBus);
            }
        private:
            struct Contract{
                virtual std::shared_ptr<API::ISystem> ConstructSystem(API::SystemID SID, Events::RequestBus& requestBus) = 0;
                virtual ~Contract(){};
            };

            template<typename T>
            struct Model : public Contract{
                T systemType;

                Model(T&& systemType) : systemType(systemType){}

                std::shared_ptr<API::ISystem> ConstructSystem(API::SystemID SID, Events::RequestBus& requestBus) override{
                    return systemType.NewSystem(SID, requestBus);
                }
            };

            std::unique_ptr<Contract> impl;
    };
}
