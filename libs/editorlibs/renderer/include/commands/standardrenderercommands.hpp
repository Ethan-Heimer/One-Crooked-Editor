#pragma once

#include "rendercommandbase.hpp"
#include "terminal.hpp"
#include <string>

namespace Rendering::Commands{
    class RenderString : public Rendering::Commands::RenderCommandBase{
        public:
            std::string string;
            unsigned int row{}, col{};

            RenderString(const Terminal::TerminalController& terminal, unsigned int row, unsigned int col, std::string string);
            void Do(CursorPosition& cursorPos, TUITexture& frameBuffer) override;
    };

    class ChangeCursorPosition : public Rendering::Commands::RenderCommandBase{
        public:
            unsigned int row{}, col{};

            ChangeCursorPosition(const Terminal::TerminalController& terminal, unsigned int row, unsigned int col);
            void Do(CursorPosition& cursorPos, TUITexture& frameBuffer);
    };
}
