#pragma once

#include "subprocess.hpp"

namespace CrookedEditor::Runtime{
    struct SubprocessConstructor{
        public:
            template<typename... Args>
            requires(std::is_same_v<const char*, Args> && ...)
            SubprocessConstructor(std::string_view process, Args&&... args) 
                : pImpl(std::make_unique<Model<Args...>>(process, std::forward<Args>(args)...)){}

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
                Model(std::string_view process, Args&&... args) 
                    : process(std::move(process)), args(std::make_tuple(std::forward<Args>(args)...)){}


                Process::Subprocess CreateSubprocess() const override{
                    return std::apply([&](auto&&... args){
                            return Process::Subprocess{process, std::forward<decltype(args)>(args)...}; 
                    }, args);
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

}
