#pragma once

#include "isystem.hpp"
#include "runtimeresourcemanager.hpp"
#include "requestbus.hpp"

namespace CrookedEditor::Runtime{
    class SystemManager final{
        public:
            SystemManager(RuntimeResourceManager& application, 
                    Events::RequestBus& requestBus);

            ~SystemManager();

            void ExecuteSystemQueue();

            void StopSystem(API::SystemID system);
            void StopAll();

            template<typename T>
            API::SystemID StartSystem(){
                return StartSystemP(T{});
            }

        protected:
            API::SystemID StartSystemP(const API::ISystem& system);

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;

    };
}
