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

            void On(request_callback_type&& callback){
                this->callback = callback;
                hasBehavior = true;
            }
    };

    template<typename Friend, typename... Args>
    class Request<Friend, void, Args...>{
        public:
            using request_return_type = void;
            using request_callback_type = std::function<void(const Args&...)>;

            friend Friend;

            void On(request_callback_type&& callback){
                callbacks.push_back(callback);
            }
            
        private:
            std::vector<request_callback_type> callbacks;  

            void Send(const Args&... args){    
                for(auto& func : callbacks){
                    func(args...);
                }
            }
            
    };
}
