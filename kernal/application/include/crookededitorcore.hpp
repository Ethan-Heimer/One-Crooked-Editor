#pragma once

#include "application.hpp"
#include "configvault.hpp"
#include "requestbus.hpp"
#include "systemmanager.hpp"
#include "terminalenviorment.hpp"

namespace CrookedEditor{
    class CrookedEditorCore{
        public:
            Events::RequestBus RequestBus;

            Application::Application Application{RequestBus}; 
            Application::ConfigVault ConfigVault{RequestBus};
            Application::TerminalEnviorment TerminalController{RequestBus};

            Application::SystemHandling::SystemManager SystemManager{Application, RequestBus};

            CrookedEditorCore(int argc, char** argv);
            ~CrookedEditorCore();
    };

}
