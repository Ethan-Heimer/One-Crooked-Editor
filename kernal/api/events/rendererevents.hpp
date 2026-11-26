#pragma once

#include "irendercommand.hpp"
#include "requestbus.hpp"

namespace CrookedEditor::API{
    struct PublishRenderCommand 
        : public Events::IEvent<Rendering::Commands::IRenderCommand>{};
}
