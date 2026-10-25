#pragma once

#include "application.hpp"
#include "configvault.hpp"
#include "events.hpp"
#include "inputhandler.hpp"
#include "pluginlauncher.hpp"
#include "systemmamager.hpp"
#include "terminal.hpp"
#include "terminalrenderer.hpp"
#include "texteditor.hpp"

namespace CrookedEditor{
    class CrookedEditorCore{
        public:
            Event<CrookedEditorCore> OnCoreQuit;

            Terminal::TerminalController TerminalController;

            Application::Application Application; 
            Application::ConfigVault ConfigVault;

            Application::InputHandler InputHandler;
            Application::TerminalRenderer Renderer;

            Application::TextEditor TextEditor;
            Application::PluginLauncher pluginLauncher;

            Application::SystemManager SystemManager;

            CrookedEditorCore(int argc, char** argv);
            ~CrookedEditorCore();
    };

}
