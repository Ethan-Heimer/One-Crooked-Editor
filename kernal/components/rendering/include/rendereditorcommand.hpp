#pragma once

#include "cursor.hpp"
#include "editorvisualstate.hpp"
#include "ieditable.h"
#include "irendercommand.hpp"
#include "tuitexture.hpp"
#include <memory>
#include <vector>

namespace CrookedEditor::Renderer {
    class RenderEditorCommand : public Rendering::Commands::IRenderCommand{
        public:
            constexpr static int lineColumnSpace = 0;
            constexpr static int verticalScrollThreshold = 0;
            constexpr static int invisableRows = 0;

            std::vector<std::string> lines;

            int cursorRow = 0;
            int cursorCol = 0;

            size_t rowOffset = 0;

            RenderEditorCommand(const Editor::IEditable& buffer, 
                    Components::TextEditorVisualState& visualState, int row, int col);

            void Do(Rendering::CursorPosition& cursorPos, Rendering::TUITexture& frameBuffer) override;

            std::unique_ptr<Rendering::Commands::IRenderCommand> Clone() const override;
    };    
}
