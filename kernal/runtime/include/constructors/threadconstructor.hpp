#pragma once

#include <functional>
#include <thread>
#include <tuple>
#include <utility>

namespace CrookedEditor::Runtime{
    struct ThreadConstructor{
        public:
            template<typename Func, typename... Args>
            ThreadConstructor(Func func, Args&&... args) 
                : pImpl(std::make_unique<Model<Func, Args&&...>>(std::move(func), 
                            std::forward<Args>(args)...)){}

            std::thread CreateThread(std::function<void()> onDoneExecuting, 
                    const std::atomic<bool>& quit) const{
                return pImpl->CreateThread(onDoneExecuting, quit);
            }
        private:
            struct Contract{
                virtual std::thread CreateThread(std::function<void()> onDoneExecuting, 
                        const std::atomic<bool>& quit) = 0;
                virtual ~Contract(){};
            };

            template<typename Func, typename... Args>
            struct Model : Contract{
                Func func;
                std::tuple<Args...> args;
                Model(Func func, Args&&... args) : func(std::move(func)),
                    args(std::make_tuple(std::forward<Args>(args)...)){}

                std::thread CreateThread(std::function<void()> onDoneExecuting,
                        const std::atomic<bool>& quit) override{

                    auto applyFunc = [&](auto&&... args){ 

                        return std::thread{[&](std::function<void()> onDone, Func f, const std::atomic<bool>& q, auto... a){
                            f(q, a...);
                            onDone();
                        }, onDoneExecuting , func, std::ref(quit), std::forward<Args>(args)...}; 

                    };

                    return std::apply(applyFunc, args);
                }
            };

            std::unique_ptr<Contract> pImpl;
    };
}
