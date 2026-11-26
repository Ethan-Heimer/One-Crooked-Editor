#pragma once

#include "requestbus.hpp"
#include "terminalinterface.hpp"

namespace CrookedEditor::Renderer{
    Rendering::TerminalInterface MakeTerminalInterface(Events::RequestBus& requestBus);
}
