#pragma once

#include "configvault.hpp"
#include "eventbus.hpp"
#include "isystem.hpp"
#include "application.hpp"
#include "systemconstructor.hpp"

namespace CrookedEditor::Application::SystemHandling{
    class SystemManager{
        public:
            SystemManager(Application& application, ConfigVault& connfigVault, 
                    Events::EventBus& eventBus);

            ~SystemManager();

            void ExecuteSystemQueue();

            template<typename T, typename... Args>
            requires SystemInterface<T>
            SystemID StartSystem(Args... args){
                return StartSystem(SystemConstructor{SystemType<T>{}, args...});
            }

            void StopSystem(SystemID system);
            void StopAll();

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;

            std::shared_ptr<ISystem> InstanciateSystem();
            SystemID StartSystem(SystemConstructor&& systemConstructor);
    };
}
