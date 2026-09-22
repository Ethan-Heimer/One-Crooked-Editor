#pragma once

#include <coroutine>
#include <future>
namespace Coroutines{
    struct LazyAwaitable{
        std::coroutine_handle<> handle;
        bool await_ready() noexcept { return false; }
        void await_suspend(std::coroutine_handle<> handle) noexcept{
            this->handle = handle;
            //handle.resume();
        }

        void await_resume() noexcept {}

        void Resume(){
            if(handle)
                handle.resume();
        }
    };

    template <typename T>
    struct EagerCoroutine{
        struct promise_type{
            T value;
            EagerCoroutine get_return_object(){return EagerCoroutine{std::coroutine_handle<promise_type>::from_promise(*this)};}
            std::suspend_never initial_suspend() noexcept {return{};}
            std::suspend_always final_suspend() noexcept {return{};}

            void return_value(T value) noexcept{
                this->value = std::move(value);
            }

            void unhandled_exception(){};

        };

        std::coroutine_handle<promise_type> handle;
        EagerCoroutine(std::coroutine_handle<promise_type> handle) : handle(handle){}

        ~EagerCoroutine(){
            if(handle)
                handle.destroy();
        }

        T Value(){
            return handle.promise().value;
        }
    };

    template <typename T>
    struct Response{
        struct promise_type{
            std::promise<T> value;
            Response get_return_object(){return Response{std::coroutine_handle<promise_type>::from_promise(*this)};}
            std::suspend_never initial_suspend() noexcept {return{};}
            std::suspend_always final_suspend() noexcept {return{};}

            void return_value(T value) noexcept{
                this->value.set_value(std::move(value));
            }

            void unhandled_exception(){};

        };

        std::coroutine_handle<promise_type> handle;
        Response(std::coroutine_handle<promise_type> handle) : handle(handle){}

        ~Response(){
            if(handle)
                handle.destroy();
        }

        T Get(){
            return handle.promise().value.get_future().get();
        }

    };
}
