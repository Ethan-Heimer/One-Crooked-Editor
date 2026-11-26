#pragma once

#include <cassert>
#include <functional>
#include <utility>
#include <vector>

#include "memberfunctiontype.hpp"

namespace Events{
    template <typename Friend, typename... Args>
    class Event{
        friend Friend;
        public:        
            using listener_lambda_type = std::function<void(const Args&...)>;
            using listener_member_func_type = MemberFunctionType<void, const Args&...>;

            Event(){};
            Event(const Event&) = delete;
            Event(Event&&) = delete;
    
            void Subscribe(listener_lambda_type&& func){
                functions.push_back({func});
            }
    
            template <typename T> 
            void Subscribe(T& reference, void(T::*function)(const Args&...)){
                memberFunctions.push_back({reference, function});
            }
    
            template <typename T>
            void Unsubscribe(const T& reference, void(T::*function)(const Args&...)){
                int index = -1; 
                for(int i = 0; i < memberFunctions.size(); i++){
                    listener_member_func_type& memberFunc = memberFunctions[i];
    
                    if(memberFunc.IsMemberOf(reference) && memberFunc.IsFunctionOf(function)){
                        index = i;
                        break;
                    }
                }
    
                if(index <= -1)
                    return;
    
                memberFunctions.erase(memberFunctions.begin() + index);
            }
    
            template <typename T>
            void UnsubscribeAll(const T& reference){
                std::vector<int> indicies;
                indicies.reserve(memberFunctions.size());
    
                for(int i = 0; i < memberFunctions.size(); i++){
                    listener_member_func_type& memberFunc = memberFunctions[i];
    
                    if(memberFunc.IsMemberOf(reference))
                        indicies.push_back(i);
                }
    
                if(indicies.size() <= 0)
                    return;
                
                int offset = 0;
                for(int i = 0; i< indicies.size(); i++){
                    memberFunctions.erase(memberFunctions.begin() + indicies[i] - offset);
                    offset++; 
                }
            }
    
            void operator+=(listener_lambda_type&& function){
                Subscribe(std::move(function));
            }
    
            void operator+=(listener_member_func_type&& memberFunction){
                memberFunctions.push_back(std::move(memberFunction));
            }
    
        protected:
            std::vector<listener_lambda_type> functions;
            std::vector<listener_member_func_type> memberFunctions;
    
            void Invoke(Args&&... args){
                for(auto& func : functions){
                    func(std::forward<Args>(args)...);
                }
    
                for(auto& func : memberFunctions){
                    func(std::forward<Args>(args)...);
                }
            }
    
            void operator()(Args&&... args){
                Invoke(std::forward<Args>(args)...);
            }
    };

}
