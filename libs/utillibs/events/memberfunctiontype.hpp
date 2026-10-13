#pragma once

#include <any>
#include <memory>
#include <assert.h>

template<typename Return, typename... FuncArgs>
class MemberFunctionType{
    public:
        template<typename T>
        MemberFunctionType(T& reference, Return(T::*function)(FuncArgs...)) 
        : impl(std::move(std::make_unique<Model<T>>(reference, function))) {}

        Return operator()(FuncArgs... args){
            return impl->Invoke(args...);
        }

        template <typename T>
        bool IsMemberOf(const T& reference){
            return impl->IsMemberOf(&reference);
        }

        template<typename T>
        bool IsFunctionOf(Return(T::*function)(FuncArgs...)){
            return impl->IsFunctionOf(function);
        }

    private:
        struct Contract{
            virtual Return Invoke(FuncArgs... args) = 0;

            virtual bool IsMemberOf(const void* refAddress) = 0;
            virtual bool IsFunctionOf(std::any possableFunctionPointer) = 0;

            virtual ~Contract(){};
        };

        template<typename T>
        struct Model : public Contract{
            T& reference;
            Return(T::*function)(FuncArgs...);

            Model(T& reference, Return(T::*function)(FuncArgs...)) 
                : reference(reference), function(function){}

            Return Invoke(FuncArgs... args) override {
                return std::invoke(function, reference, args...);
            }

            bool IsMemberOf(const void* refAddress) override {
                return &reference == refAddress;
            }

            bool IsFunctionOf(std::any possableFunctionPointer){
                assert(possableFunctionPointer.type() == typeid(decltype(function)));

                auto functionPointer = std::any_cast<Return(T::*)(FuncArgs...)>(possableFunctionPointer);

                return functionPointer == function;
            }
        };

        std::unique_ptr<Contract> impl;
};
