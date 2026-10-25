#pragma once

#include "isystem.hpp"
#include "systemstartoptions.hpp"

#include "application.hpp"

#include <memory>

namespace CrookedEditor::Application{
    class SystemManager{
        public:
            SystemManager(class Application& application);
            ~SystemManager();

            void ExecuteSystemQueue();

            template<typename T, typename... Args>
            SystemID StartSystem(Args... args){
                return StartSystem(std::make_shared<T>(args...));
            }

            void StopSystem(SystemID system);
            void StopAll();

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;

            SystemID StartSystem(std::shared_ptr<ISystem> system);
    };
}
