#pragma once

#include <cstddef>
#include "application.hpp"
#include "configvault.hpp"
#include "eventbus.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

namespace CrookedEditor::Application::SystemHandling{
    using SystemID = size_t;

    class ISystem{
        public:
            ISystem(SystemID SID, Application& application, 
                    ConfigVault& configVault, Events::EventBus& eventBus) 
                : SID(SID), application(application), configVault(configVault),
                eventBus(eventBus){};

            virtual ~ISystem(){}

            virtual SystemStartOptions Start() = 0;
            virtual bool Update() = 0;
            virtual void Stop(SystemTerminateReasons reason) = 0;

        protected:
            SystemID SID;
            Application& application;
            ConfigVault& configVault;
            Events::EventBus& eventBus;

    };

    template<typename T>
    concept SystemInterface = requires (T x) { 
            x.Start();
            x.Update();
            x.Stop(SystemTerminateReasons::ExecutionEnded);
    };
}
