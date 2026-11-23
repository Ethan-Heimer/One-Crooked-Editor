#pragma once

#include "cursor.hpp"
#include "tuitexture.hpp"
#include <memory>
namespace Rendering::Commands{
    class RenderCommand{
        public:
            RenderCommand(){};

            template<typename T> 
            RenderCommand(T value) : impl(std::make_unique<Model<T>>(std::move(value))){};
            void Do(CursorPosition& position, TUITexture& frameBuffer){
                if(impl)
                    impl->Do(position, frameBuffer);
            }

            void operator()(CursorPosition& position, TUITexture& frameBuffer){
                Do(position, frameBuffer);
            }

        private:
            struct Contract{
                virtual void Do(CursorPosition& position, TUITexture& frameBuffer) = 0;
                virtual ~Contract(){};
            };

            template <typename T>
            struct Model : Contract{
                T value;
                Model(T value) : value(std::move(value)){}

                void Do(CursorPosition& position, TUITexture& frameBuffer) override{
                    value.Do(position, frameBuffer);
                }
            };

            std::unique_ptr<Contract> impl;
    };
}
