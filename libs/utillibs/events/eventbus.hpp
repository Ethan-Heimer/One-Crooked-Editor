#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>

#include <typeindex>

#include "imessage.hpp"

namespace Events{
    template<typename T>
    struct ITopic{
        using message_param_type = T;
    };
    
    class EventBus{
        public:
            template<typename T>
            void AddTopic(){
                std::unique_lock<std::mutex> lock{topicLock};
                channels[typeid(T)] = 
                    std::make_unique<MessageChannel<typename T::message_param_type>>();
            }
    
            template<typename T>
            void AddTopicListener(std::function<void(typename T::message_param_type)> callback){
                std::unique_lock<std::mutex> lock{topicLock};
                
                if(!channels.contains(typeid(T)))
                    return;
    
                channels[typeid(T)]->AddListener(callback);
            }
    
            template<typename T>
            void AddTopicAndListen(std::function<void(typename T::message_param_type)> callback){
                AddTopic<T>();
                AddTopicListener<T>(callback);
            }
                
            template<typename T>
            void InvokeTopic(typename T::message_param_type param){
                std::unique_lock<std::mutex> lock{topicLock};
    
                if(!channels.contains(typeid(T)))
                    return;
    
                channels[typeid(T)]->Invoke(param);
            }
     
        private:
            std::map<std::type_index, std::unique_ptr<IMessageChannel>> channels;
            std::mutex topicLock;
    
    };
}
