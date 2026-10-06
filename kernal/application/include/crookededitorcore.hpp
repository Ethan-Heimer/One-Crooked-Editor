#pragma once

#include "application.hpp"
#include "configvault.hpp"
#include "inputhandler.hpp"
#include "terminal.hpp"
#include "terminalrenderer.hpp"
#include "texteditor.hpp"

namespace CrookedEditor{
    class CrookedEditorCore{
        public:
            Terminal::TerminalController TerminalController;

            Application::Application Application; 
            Application::ConfigVault ConfigVault;

            Application::InputHandler InputHandler;
            Application::TerminalRenderer Renderer;

            Application::TextEditor TextEditor;

            CrookedEditorCore(int argc, char** argv);
            ~CrookedEditorCore();

            bool CoreQuit();
    };

}
