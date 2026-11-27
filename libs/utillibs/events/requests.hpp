#pragma once

#include <cassert>
#include <expected>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace Events{
    template<typename Friend, typename Ret, typename... Args>
    class Request{
        public:
            using request_return_type = std::expected<Ret, std::string>;
            using request_callback_type = std::function<request_return_type(const Args&...)>;
            using request_success_type = Ret;

            friend Friend;

            request_return_type Send(Args&&... args){    
                if(!hasBehavior){
                   return std::unexpected("Request Has No Behavior");
                }

                return callback(std::forward<Args>(args)...);
            }
            
        private:
            bool hasBehavior = false;
            request_callback_type callback;  

            void On(const request_callback_type& callback){
                this->callback = callback;
                hasBehavior = true;
            }

            void RemoveCallback(const request_callback_type& callback){
                if(this->callback.target_type() == callback.target_type()){
                    this->callback = nullptr;
                }
            }
    };

    template<typename Friend, typename... Args>
    class Request<Friend, void, Args...>{
        public:
            using request_success_type = void;
            using request_return_type = std::expected<void, std::string>;
            using request_callback_type = std::function<void(const Args&...)>;

            friend Friend;

            void On(const request_callback_type& callback){
                callbacks.push_back(callback);
            }

            void RemoveCallback(const request_callback_type& callback){
                std::erase_if(callbacks, [&](const request_callback_type& element){
                    return element.target_type() == callback.target_type();
                });
            }
            
        private:
            std::vector<request_callback_type> callbacks;  

            request_return_type Send(const Args&... args){    
                for(auto& func : callbacks){
                    func(args...);
                }

                return {};
            }
            
    };
}
