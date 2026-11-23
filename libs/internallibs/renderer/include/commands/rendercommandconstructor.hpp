#pragma once

#include "rendercommandtype.hpp"
#include <memory>
#include <tuple>

namespace Rendering::Commands{
    class IRenderCommandConstructor{
        public:
            virtual RenderCommand Construct() = 0;
            virtual ~IRenderCommandConstructor(){};
    };

    template<typename T, typename... Args>
    class RenderCommandCostructor final : public IRenderCommandConstructor{
        public:
            RenderCommandCostructor(Args... args){
                this->args = std::make_tuple(args...);
            }

            RenderCommand Construct() override{
               return std::apply([&](Args... args){
                    auto command = RenderCommand{T{args...}};
                    return command;
                }, args);
            }

        private:
            std::tuple<Args...> args;
    };

    template<typename T, typename... Args>
    std::shared_ptr<IRenderCommandConstructor> CreateCommandConstructor(Args&... args){
        return std::make_shared<RenderCommandCostructor<T, Args...>>(args...);
    }
}
