#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include <sys/socket.h>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "events.hpp"
#include "subprocess.hpp"

namespace CrookedEditor::Application{
    using ThreadHandle = int;
    using SubprocessHandle = int;

    struct ThreadConstructor{
        public:
            template<typename Func, typename... Args>
            ThreadConstructor(Func func, Args... args) 
                : pImpl(std::make_unique<Model<Func, Args...>>(std::move(func), args...)){}

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
                Model(Func func, Args... args) : func(std::move(func)), args(std::make_tuple(args...)){}

                std::thread CreateThread(std::function<void()> onDoneExecuting,
                        const std::atomic<bool>& quit) override{

                    auto applyFunc = [&](auto... args){ 

                        return std::thread{[&](std::function<void()> onDone, Func f, const std::atomic<bool>& q, auto... a){
                            f(q, a...);
                            onDone();
                        }, onDoneExecuting , func, std::ref(quit), args...}; 

                    };

                    return std::apply(applyFunc, args);
                }
            };

            std::unique_ptr<Contract> pImpl;
    };

    struct SubprocessConstructor{
        public:
            template<typename... Args>
            requires(std::is_same_v<const char*, Args> && ...)
            SubprocessConstructor(std::string_view process, Args... args) 
                : pImpl(std::make_unique<Model<Args...>>(process, args...)){}

            Process::Subprocess CreateSubprocess() const{
                return pImpl->CreateSubprocess();
            }
        private:
            struct Contract{
                virtual Process::Subprocess CreateSubprocess() const = 0;
                virtual ~Contract(){};
            };

            template<typename... Args>
            struct Model : Contract{
                std::string_view process;
                std::tuple<Args...> args;
                Model(std::string_view process, Args... args) : process(std::move(process)), args(std::make_tuple(args...)){}


                Process::Subprocess CreateSubprocess() const override{
                    return std::apply([&](auto... args){return Process::Subprocess{process, (args, ...)}; }, args);
                }
            };

            template<>
            struct Model<> : Contract{
                std::string_view process;
                Model(std::string_view process) : process(std::move(process)){}

                Process::Subprocess CreateSubprocess() const override{
                    return Process::Subprocess{process};
                }
            };

            std::unique_ptr<Contract> pImpl;
    };

    struct SubprocessInfo{
        SubprocessHandle subprocessHandle;   

        Process::FileDescripter inputFd;
        Process::FileDescripter outputFd;
    };

    struct MemoryUsageData{
        double vmUsage;
        double residentSet;
    };

    class Application{
        public:
            constexpr static int EmpytHandle = -1;

            Events::Event<Application> OnQuit;

            Application();
            ~Application();
            Application(const Application&) = delete;
            Application(Application&&) = delete;

            Application& operator=(const Application&) = delete;
            Application& operator=(Application&&) = delete;

            void CleanUp();

            template<typename Func, typename... Args>
            ThreadHandle SpawnThread(Func func, Args... args) const {
                return SpawnThread({func, args...});
            }
            
            template<typename... Args>
            requires(std::is_same_v<const char*, Args> && ...)
            SubprocessInfo SpawnSubprocess(std::string_view process, Args... args) const {
                return SpawnSubprocess(SubprocessConstructor{process, args...});
            }

            ThreadHandle SpawnThread(ThreadConstructor threadConstructor) const;
            SubprocessInfo SpawnSubprocess(SubprocessConstructor process) const;

            Process::FileDescripter GetInputFileDescriptor(SubprocessHandle handle) const;
            Process::FileDescripter GetOutputFileDescriptor(SubprocessHandle handle) const;

            void Kill(SubprocessHandle handle);
            void KillThread(ThreadHandle handle);

            bool DoesThreadExist(ThreadHandle handle);

            void PrintLog(std::string_view message);

            MemoryUsageData MemoryUsage();

            int GetThreadCount();
            std::vector<int> GetThreadIds();

            void Quit();
 
        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
