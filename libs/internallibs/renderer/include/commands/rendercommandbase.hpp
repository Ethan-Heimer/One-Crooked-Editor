#pragma once

#include "cursor.hpp"
#include "tuitexture.hpp"

namespace Rendering::Commands{
    class RenderCommandBase{
        public: 
            size_t CommandID{};

            virtual ~RenderCommandBase(){}
            virtual void Do(CursorPosition& cursorPos, TUITexture& frameBuffer) = 0;

            void operator()(CursorPosition& cursorPos, TUITexture& frameBuffer){
                Do(cursorPos, frameBuffer);
            }
    };
}
