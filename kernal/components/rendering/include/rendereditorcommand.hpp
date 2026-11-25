#pragma once

#include "cursor.hpp"
#include "editorvisualstate.hpp"
#include "irendercommand.hpp"
#include "tuitexture.hpp"
#include <memory>

namespace CrookedEditor::Renderer {
    class RenderEditorCommand : public Rendering::Commands::IRenderCommand{
        public:
            const std::shared_ptr<const Components::TextEditorVisualState> renderingState;

            RenderEditorCommand(std::shared_ptr<const Components::TextEditorVisualState> renderingState);

            void Do(Rendering::CursorPosition& cursorPos, Rendering::TUITexture& frameBuffer) override;

            std::unique_ptr<Rendering::Commands::IRenderCommand> Clone() const override;
    };    
}
