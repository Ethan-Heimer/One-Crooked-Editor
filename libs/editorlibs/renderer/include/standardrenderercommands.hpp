#pragma once

#include "rendercommand.hpp"
#include <string>

namespace Rendering::Commands{
    class RenderString : public Rendering::Commands::RenderCommand{
        public:
            std::string string;
            int row{}, col{};

            RenderString(int row, int col, std::string string);
            void Do(CursorPosition& cursorPos, TUITexture& frameBuffer) override;
    };

    class ChangeCursorPosition : public Rendering::Commands::RenderCommand{
        public:
            int row{}, col{};

            ChangeCursorPosition(int row, int col);
            void Do(CursorPosition& cursorPos, TUITexture& frameBuffer);
    };
}
