#pragma once

#include <cassert>
#include <functional>
#include <vector>

#include "memberfunctiontype.hpp"

namespace Events{
    template <typename Friend, typename... Args>
    class Event{
        friend Friend;
        public:        
            Event(){};
            Event(const Event&) = delete;
            Event(Event&&) = delete;
    
            void Subscribe(std::function<void(Args...)>&& func){
                functions.push_back({func});
            }
    
            template <typename T> 
            void Subscribe(T& reference, void(T::*function)(Args...)){
                memberFunctions.push_back({reference, function});
            }
    
            template <typename T>
            void Unsubscribe(const T& reference, void(T::*function)(Args...)){
                int index = -1; 
                for(int i = 0; i < memberFunctions.size(); i++){
                    MemberFunctionType<void, Args...>& memberFunc = memberFunctions[i];
    
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
                    MemberFunctionType<void, Args...>& memberFunc = memberFunctions[i];
    
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
    
            void operator+=(std::function<void(Args...)>&& function){
                Subscribe(std::move(function));
            }
    
            void operator+=(MemberFunctionType<void, Args...>&& memberFunction){
                memberFunctions.push_back(std::move(memberFunction));
            }
    
        protected:
            std::vector<std::function<void(Args...)>> functions;
            std::vector<MemberFunctionType<void, Args...>> memberFunctions;
    
            void Invoke(Args... args){
                for(auto& func : functions){
                    func(args...);
                }
    
                for(auto& func : memberFunctions){
                    func(args...);
                }
            }
    
            void operator()(Args... args){
                Invoke(args...);
            }
    };

}
