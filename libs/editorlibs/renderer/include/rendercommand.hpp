#pragma once

#include "cursor.hpp"
#include "tuitexture.hpp"

namespace Rendering::Commands{
    class RenderCommand{
        public: 
            virtual ~RenderCommand(){}
            virtual void Do(CursorPosition& cursorPos, TUITexture& frameBuffer) = 0;

            void operator()(CursorPosition& cursorPos, TUITexture& frameBuffer){
                Do(cursorPos, frameBuffer);
            }
    };
}
