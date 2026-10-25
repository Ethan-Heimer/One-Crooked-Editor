#pragma once

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <mutex>

#include "events.hpp"
#include <optional>
#include <typeindex>
#include <utility>

struct Result{
    bool sucsessfull = false;
    std::optional<const std::type_index> expectedType;
};

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

template<typename T>
struct ITopic{
    using message_param_type = T;
};

template<typename T>
concept IsTopic = requires (T x) {
    T::message_param_type;
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
