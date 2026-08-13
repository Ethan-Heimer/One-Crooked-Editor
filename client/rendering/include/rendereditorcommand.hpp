#pragma once

#include "cursor.hpp"
#include "ieditable.h"
#include "rendercommandbase.hpp"
#include "terminal.hpp"
#include "tuitexture.hpp"
#include <string>
#include <vector>

namespace CrookedEditor::Renderer {
    struct EditorRenderingState{
        int lastRowNumber;
        int currentLineViewOffset;
        int colOffset;
    };

    class RenderEditorCommand : public Rendering::Commands::RenderCommandBase{
        public:
            const int lineColumnSpace = 4;
            const int verticalScrollThreshold = 5;
            const int invisableRows = 2;

            std::vector<std::string> lines;
            std::vector<std::string> lineNumbers;
            std::vector<bool> isCurrentLine;

            int cursorCol;
            int row, col;

            int& colOffset;
                
            int lineNumberPaddingWidth{};
            int lineNumberColumnWidth{};

            RenderEditorCommand(const Terminal::TerminalController& terminalController, const Editor::IEditable& buffer, EditorRenderingState& renderingData);
            void Do(Rendering::CursorPosition& cursorPos, Rendering::TUITexture& frameBuffer) override;
    };
}
