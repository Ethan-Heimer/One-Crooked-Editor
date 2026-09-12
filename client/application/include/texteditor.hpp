#pragma once

#include "application.hpp"
#include "configvault.hpp"
#include "editorrenderingstate.hpp"
#include "ieditable.h"
#include <memory>

namespace CrookedEditor::Application{
    class TextEditor{
        public:
            TextEditor(Application& app, ConfigVault& config);
            ~TextEditor();

            void Start(std::string_view fileName, std::function<int()> getNextKey, 
                std::function<void(const Editor::IEditable& buffer, Renderer::EditorRenderingState& renderingState)> renderEditor);

            void Stop();

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
