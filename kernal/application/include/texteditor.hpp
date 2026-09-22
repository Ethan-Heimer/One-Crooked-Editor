#pragma once

#include "application.hpp"
#include "configvault.hpp"
#include "editorrenderingstate.hpp"
#include "terminal.hpp"
#include <memory>
#include <string>
#include <vector>

namespace CrookedEditor::Application{
    class TextEditor{
        public:
            TextEditor(Application& app, Terminal::TerminalController& terminalController, ConfigVault& config);
            ~TextEditor();

            void Start(std::string_view fileName, std::function<int()> getNextKey, 
                std::function<void(Renderer::EditorRenderingState renderingState, 
                    std::vector<std::string>&& lines, std::vector<std::string>&& lineNumbers,
                    int currentLine)> renderEditor);

            void Stop();

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
