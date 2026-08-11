#pragma once

#include "tuitexture.hpp"
namespace Rendering::Commands{
    class RenderCommand{
        public: 
            virtual ~RenderCommand(){}
            virtual void Do(TUITexture& frameBuffer) = 0;

            void operator()(TUITexture& frameBuffer){
                Do(frameBuffer);
            }
    };
}
