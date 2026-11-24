#pragma once

#include "runtimeresourcemanager.hpp"
#include "configvault.hpp"
#include "requestbus.hpp"
#include "systemmanager.hpp"
#include "terminalenviorment.hpp"

namespace CrookedEditor{
    class RuntimeCore{
        public:
            Events::RequestBus RequestBus;

            Runtime::RuntimeResourceManager Application{RequestBus}; 
            Runtime::ConfigVault ConfigVault{RequestBus};
            Runtime::TerminalEnviorment TerminalController{RequestBus};

            Runtime::SystemManager SystemManager{Application, RequestBus};

            RuntimeCore(int argc, char** argv);
            ~RuntimeCore();
    };

}
