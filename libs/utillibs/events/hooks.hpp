#pragma once

#include "memberfunctiontype.hpp"
#include <functional>
#include <optional>
#include <utility>
#include <variant>
template<typename Friend, typename Return, typename... Args>
class SoliatryHook{
    public:
        using LambdaType = std::function<Return(Args...)>;
        using HookFunctionStoreType = std::optional<std::variant<LambdaType, MemberFunctionType<Return, Args...>>>;

        friend Friend; 

        void SetCallback(LambdaType&& func){
            callback = func; 
        }

        template<typename T>
        void SetCallback(T& ref, Return(T::*func)(Args...)){
            callback = MemberFunctionType<Return, Args...>{ref, func};
        }

        void operator=(LambdaType&& func){
            SetCallback(std::forward<LambdaType>(func));
        }

        void operator=(MemberFunctionType<Return, Args...>&& func){
            callback = func;
        }

    private:
        HookFunctionStoreType callback;
    
        Return Invoke(Args... args){
            Return value{};
            if(callback.has_value()){
                std::visit([&](auto& val){
                    value = val(args...);
                }, callback.value());

                return value;
            }

            return value;
        }

        Return operator()(Args... args){
            return Invoke(std::move(args)...);
        }

};
