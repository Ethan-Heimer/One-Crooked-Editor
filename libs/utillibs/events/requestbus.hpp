#pragma once

#include "irequestchannel.hpp"
#include <any>
#include <map>
#include <memory>
#include <mutex>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <utility>

/*
 * All in one, copy free, framework for event bus 
 * One bus type handles requests, events, and notifications 
 *
 * requests: returns data back to caller, one listener, meny callers
 * events: "void request", no data back to the sender, meny listeners and callers
 * notification: "paramater-less event" meny listeners and callers
 */

namespace Events{
    template<typename Ret, typename... Args>
    struct IRequest{
        using request_params_type = std::tuple<Args...>;
        using request_successful_type = Ret;
        using request_channel_type = RequestChannel<request_successful_type, const Args&...>;

        using request_return_type = request_channel_type::RequestType::request_return_type;
        using request_callback_type = request_channel_type::RequestType::request_callback_type;

        constexpr static std::size_t arg_cout = sizeof...(Args);
    };
    
    template<typename... Args>
    using IEvent = IRequest<void, Args...>;
    using INotif = IEvent<>;

    template<typename Tuple>
    struct RequestSender;
    
    template<typename... Args>
    struct RequestSender<std::tuple<Args...>>{
        template<typename Channel>
        auto Send(Channel& channel, const Args&... args){
            return channel.Send(args...);
        }
    };
    
    class RequestBus{
        public: 
            template<typename T>
            void AddRequest(typename T::request_callback_type&& callback){
                std::unique_lock<std::mutex> lock{topicLock};

                AddChannelFor<T>(std::move(callback)); 
            }

            template<typename T>
            requires std::is_same_v<typename T::request_return_type, void>
            void AddRequest(){
                std::unique_lock<std::mutex> lock{topicLock};
                AddChannelFor<T>(); 
            }
    
                
            template<typename Request, typename... Args>
            [[nodiscard("Return of SendRequest must be observed")]]
            auto SendRequest(Args&&... params) -> typename Request::request_return_type{
                std::unique_lock<std::mutex> lock{topicLock};

                auto& channel = GetChannelFor<Request>();

                RequestSender<typename Request::request_params_type> sender;
                return sender.Send(channel, std::forward<Args>(params)...); 
            }

            template <typename T>
            requires std::is_same_v<typename T::request_return_type, void>
            void ListenForEvent(typename T::request_callback_type&& callback){
                std::unique_lock<std::mutex> lock{topicLock};

                auto& channel = GetChannelFor<T>(); 
                channel.On(std::move(callback));
            }
 
        private:
            std::map<std::type_index, std::any> channels;
            std::mutex topicLock;

            template <typename T>
            void AddChannelFor(typename T::request_callback_type&& callback){
                auto channel = std::make_shared<typename T::request_channel_type>();

                channel->On(std::move(callback));
                channels.insert({typeid(T), channel});
            }

            template <typename T>
            requires std::is_same_v<typename T::request_return_type, void>
            void AddChannelFor(){
                auto channel = std::make_shared<typename T::request_channel_type>();
                channels.insert({typeid(T), channel});
            }

            template <typename T>
            auto GetChannelFor() -> typename T::request_channel_type&{
                auto channel_ptr = std::any_cast<std::shared_ptr
                    <typename T::request_channel_type>>(channels[typeid(T)]);

                return *channel_ptr;
            }
    };
}
