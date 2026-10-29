#pragma once

#include "application.hpp"
#include "configvault.hpp"
#include "eventbus.hpp"
#include "events.hpp"
#include "inputhandler.hpp"
#include "pluginlauncher.hpp"
#include "systemmanager.hpp"
#include "terminal.hpp"
#include "terminalrenderer.hpp"
#include "texteditor.hpp"

namespace CrookedEditor{
    class CrookedEditorCore{
        public:
            Events::Event<CrookedEditorCore> OnCoreQuit;

            Terminal::TerminalController TerminalController;

            Application::Application Application; 
            Application::ConfigVault ConfigVault;
            Events::EventBus eventBus;

            Application::InputHandler InputHandler;
            Application::TerminalRenderer Renderer;

            Application::TextEditor TextEditor;
            Application::PluginLauncher pluginLauncher;

            Application::SystemHandling::SystemManager SystemManager;

            CrookedEditorCore(int argc, char** argv);
            ~CrookedEditorCore();
    };

}
