#pragma once

#include <any>
#include <string>
#include <typeindex>
#include <utility>
#include "events.hpp"

namespace Events{
    class IMessageChannel{
        public:
            virtual void AddListener(std::any param) = 0;
            virtual void Invoke(std::any param) = 0;
    
            virtual std::string GetParamTypeName() = 0;
            virtual std::type_index GetParamTypeInfo() = 0;
    };
    
    template<typename Param>
    class MessageChannel : public IMessageChannel{
        public:
            Event<MessageChannel, Param> event;
    
            void AddListener(std::any listener) override{
                try{
                    std::function<void(Param)> func = std::any_cast<std::function<void(Param)>>(listener);
                    event += std::move(func);
                } catch(const std::bad_any_cast& e) {}
            }
    
            void Invoke(std::any args) override{
                event.Invoke(std::any_cast<Param>(args));
            }
    
            std::string GetParamTypeName() override{
               return typeid(Param).name();
            }
    
            std::type_index GetParamTypeInfo() override{
               return typeid(Param);
            }
    };
}
