#pragma once

#include "application.hpp"
#include "requestbus.hpp"
#include "systemconstructor.hpp"

namespace CrookedEditor::Application::SystemHandling{
    class SystemManager final{
        public:
            SystemManager(Application& application, Events::RequestBus& requestBus);

            ~SystemManager();

            void ExecuteSystemQueue();

            void StopSystem(SystemID system);
            void StopAll();

            template<typename T>
            SystemID StartSystem(){
                return StartSystemP(SystemConstructor{SystemType<T>{}});
            }

        protected:
            SystemID StartSystemP(SystemConstructor&& systemConstructor);

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;

    };
}
