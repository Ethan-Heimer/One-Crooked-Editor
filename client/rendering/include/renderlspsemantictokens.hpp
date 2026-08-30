#pragma once

#include "rendercommandbase.hpp"
#include "editorrenderingstate.hpp"
#include <vector>

namespace CrookedEditor::Renderer {
    class RenderSemanticTokensCommand : public Rendering::Commands::RenderCommandBase{
        public:
            const std::vector<int>& tokens;
            const EditorRenderingState& editorRenderingState;

            RenderSemanticTokensCommand(const Terminal::TerminalController& terminalController, const std::vector<int>& tokens, const EditorRenderingState& editorRenderingState);
            void Do(Rendering::CursorPosition& cursorPos, Rendering::TUITexture& frameBuffer) override;
    };
}
