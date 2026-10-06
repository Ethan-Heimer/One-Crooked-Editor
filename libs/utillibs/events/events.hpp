#pragma once

#include <any>
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

template<typename... FuncArgs>
class MemberFunctionType{
    public:
        template<typename T>
        MemberFunctionType(T& reference, void(T::*function)(FuncArgs...)) 
        : impl(std::move(std::make_unique<Model<T>>(reference, function))) {}

        void operator()(FuncArgs... args){
            impl->Invoke(args...);
        }

        template <typename T>
        bool IsMemberOf(const T& reference){
            return impl->IsMemberOf(&reference);
        }

        template<typename T>
        bool IsFunctionOf(void(T::*function)(FuncArgs...)){
            return impl->IsFunctionOf(function);
        }

    private:
        struct Contract{
            virtual void Invoke(FuncArgs... args) = 0;
            virtual bool IsMemberOf(const void* refAddress) = 0;

            virtual bool IsFunctionOf(std::any possableFunctionPointer) = 0;
        };

        template<typename T>
        struct Model : public Contract{
            T& reference;
            void(T::*function)(FuncArgs...);
            Model(T& reference, void(T::*function)(FuncArgs...)) 
                : reference(reference), function(function){}

            void Invoke(FuncArgs... args) override {
                std::invoke(function, reference, args...);
            }

            bool IsMemberOf(const void* refAddress) override {
                return &reference == refAddress;
            }

            bool IsFunctionOf(std::any possableFunctionPointer){
                assert(possableFunctionPointer.type() == typeid(decltype(function)));

                auto functionPointer = std::any_cast<void(T::*)(FuncArgs...)>(possableFunctionPointer);

                return functionPointer == function;
            }
        };

        std::unique_ptr<Contract> impl;
};

template <typename Friend, typename... Args>
class Event{
    friend Friend;
    public:        
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
                MemberFunctionType<Args...>& memberFunc = memberFunctions[i];

                if(memberFunc.IsMemberOf(reference) && memberFunc.IsFunctionOf(function)){
                    index = i;
                    break;
                }
            }

            if(index <= -1)
                return;

            memberFunctions.erase(memberFunctions.begin() + index);
        }

    protected:
        std::vector<std::function<void(Args...)>> functions;
        std::vector<MemberFunctionType<Args...>> memberFunctions;

        void Invoke(Args... args){
            for(auto& func : functions){
                func(args...);
            }

            for(auto& func : memberFunctions){
                func(args...);
            }
        }
};
