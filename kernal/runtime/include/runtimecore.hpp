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

            Runtime::RuntimeResourceManager ResourceManager{RequestBus}; 
            Runtime::ConfigVault ConfigVault{RequestBus};
            Runtime::TerminalEnviorment TerminalController{RequestBus};

            Runtime::SystemManager SystemManager{ResourceManager, RequestBus};

            RuntimeCore(int argc, char** argv);
    };

}
