#pragma once

#include "cursor.hpp"
#include "tuitexture.hpp"
#include <memory>

namespace Rendering::Commands{
    class IRenderCommand{
        public: 
            size_t CommandID{};

            virtual ~IRenderCommand(){}
            virtual void Do(CursorPosition& cursorPos, TUITexture& frameBuffer) = 0;

            virtual std::unique_ptr<IRenderCommand> Clone() const = 0;

            void operator()(CursorPosition& cursorPos, TUITexture& frameBuffer){
                Do(cursorPos, frameBuffer);
            }

            
    };
}
