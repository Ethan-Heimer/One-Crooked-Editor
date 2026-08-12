#pragma once

#include "cursor.hpp"
#include "terminal.hpp"
#include "tuitexture.hpp"

namespace Rendering::Commands{
    class RenderCommandBase{
        public: 
            RenderCommandBase(const Terminal::TerminalController& terminalController){};

            virtual ~RenderCommandBase(){}
            virtual void Do(CursorPosition& cursorPos, TUITexture& frameBuffer) = 0;

            void operator()(CursorPosition& cursorPos, TUITexture& frameBuffer){
                Do(cursorPos, frameBuffer);
            }
    };
}
