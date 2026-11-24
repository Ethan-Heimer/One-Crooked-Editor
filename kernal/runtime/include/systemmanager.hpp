#pragma once

#include "runtimeresourcemanager.hpp"
#include "requestbus.hpp"
#include "systemconstructor.hpp"

namespace CrookedEditor::Runtime{
    class SystemManager final{
        public:
            SystemManager(RuntimeResourceManager& application, Events::RequestBus& requestBus);

            ~SystemManager();

            void ExecuteSystemQueue();

            void StopSystem(API::SystemID system);
            void StopAll();

            template<typename T>
            API::SystemID StartSystem(){
                return StartSystemP(SystemConstructor{SystemType<T>{}});
            }

        protected:
            API::SystemID StartSystemP(SystemConstructor&& systemConstructor);

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;

    };
}
