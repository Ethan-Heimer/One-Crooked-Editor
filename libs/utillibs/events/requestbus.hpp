#pragma once

#include "irequestchannel.hpp"

#include <any>
#include <atomic>
#include <expected>
#include <format>
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

    struct LockKeeper{
        std::unique_lock<std::mutex> lock;
        std::atomic<bool>& doWait;

        LockKeeper(std::atomic<bool>& doWait, std::mutex& mtx)
        : doWait(doWait){
            doWait.store(true, std::memory_order_release);
            lock = std::unique_lock<std::mutex>{mtx};
        }

        ~LockKeeper(){
            lock.unlock(); 
            doWait.store(false, std::memory_order_release);
        }
    };

    class RequestBus{
        public: 
            template<typename T>
            [[nodiscard("Result should be observed")]]
            std::expected<void, std::string> AddRequest(const typename T::request_callback_type& callback){
                LockKeeper lock{doWait, mtx};

                if(channels.contains(typeid(T)))
                    return std::unexpected<std::string>{Error<T>("Request is already regestered.")};

                AddChannelFor<T>(callback); 
                return {};
            }

            template<typename T>
            requires std::is_same_v<typename T::request_successful_type, void>
            [[nodiscard("Result should be observed")]]
            std::expected<void, std::string> AddRequest(){
                LockKeeper lock{doWait, mtx};

                if(channels.contains(typeid(T)))
                    return std::unexpected<std::string>{Error<T>("Request is already regestered.")};

                AddChannelFor<T>(); 
                return {};
            }
    
            template<typename T>
            [[nodiscard("Result should be observed")]]
            std::expected<void, std::string> RemoveRequest(){
                LockKeeper lock{doWait, mtx};

                if(!channels.contains(typeid(T)))
                    return std::unexpected<std::string>{Error<T>("Request not found")};

                channels.erase(typeid(T));
                return {};
            };

            template <typename T>
            requires std::is_same_v<typename T::request_successful_type, void>
            [[nodiscard("Result should be observed")]]
            std::expected<void, std::string> RemoveEventListener(const typename T::request_callback_type& callback){
                LockKeeper lock{doWait, mtx};

                if(!channels.contains(typeid(T)))
                    return std::unexpected<std::string>{Error<T>("Request not found")};

                auto& channel = GetChannelFor<T>(); 
                channel.RemoveCallback(callback);
                return {};
            }

            template <typename T>
            requires std::is_same_v<typename T::request_successful_type, void>
            [[nodiscard("Result should be observed")]]
            std::expected<void, std::string> ListenForEvent(const typename T::request_callback_type& callback){
                WaitUntilSafe();

                if(!channels.contains(typeid(T)))
                    return std::unexpected<std::string>{Error<T>("Request not found")};

                auto& channel = GetChannelFor<T>(); 
                channel.On(callback);

                return {};
            }
     
            template<typename Request, typename... Args>
            [[nodiscard("Result should be observed")]]
            auto SendRequest(Args&&... params) -> typename Request::request_return_type{
                WaitUntilSafe();

                if(!channels.contains(typeid(Request)))
                    return std::unexpected<std::string>{Error<Request>("Request not found")};

                auto& channel = GetChannelFor<Request>();

                RequestSender<typename Request::request_params_type> sender;
                return sender.Send(channel, std::forward<Args>(params)...); 
            }


        private:
            std::map<std::type_index, std::any> channels;

            std::mutex mtx;
            std::atomic<bool> doWait;

            template <typename T>
            void AddChannelFor(const typename T::request_callback_type& callback){
                auto channel = std::make_shared<typename T::request_channel_type>();

                channel->On(callback);
                channels.insert({typeid(T), channel});
            }

            template <typename T>
            requires std::is_same_v<typename T::request_successful_type, void>
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

            template <typename T>
            constexpr std::string Error(const std::string& message){
                return std::format("Request Bus Error: {} {}", typeid(T).name(), message);
            }

            void WaitUntilSafe(){
                if(doWait.load(std::memory_order_acquire))
                    std::unique_lock<std::mutex> lock{mtx};
            }
    };
}
