#pragma once

#include "requestbus.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"
#include "isystem.hpp"
#include <memory>

namespace CrookedEditor::Systems{
    class InputHandler final : public API::ISystem{
        public:
            using API::ISystem::ISystem;

            API::SystemStartOptions Start();
            bool Update();
            void Stop(API::SystemTerminateReasons reason);

            std::shared_ptr<API::ISystem> Instanciate(API::SystemID SID, 
                    Events::RequestBus* requestBus) const;
    };
}
