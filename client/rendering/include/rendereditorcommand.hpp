#pragma once

#include "cursor.hpp"
#include "ieditable.h"
#include "rendercommandbase.hpp"
#include "terminal.hpp"
#include "tuitexture.hpp"
#include <string>
#include <vector>

#include "editorrenderingstate.hpp"

namespace CrookedEditor::Renderer {
    class RenderEditorCommand : public Rendering::Commands::RenderCommandBase{
        public:
            const EditorRenderingState renderingState;

            std::vector<std::string> lines;
            std::vector<std::string> lineNumbers;
            int currentLine;

            int row, col;

            RenderEditorCommand(const Terminal::TerminalController& terminalController, EditorRenderingState renderingData,
                    std::vector<std::string>&& lines, std::vector<std::string>&& lineNumbers, int currentLine);

            void Do(Rendering::CursorPosition& cursorPos, Rendering::TUITexture& frameBuffer) override;
    };
}
