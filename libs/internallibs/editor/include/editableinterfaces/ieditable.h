#pragma once

#include "ieditablecursorcommands.h"
#include "ieditableeditcommands.h"

namespace Editor{
    class IEditable : public IEditableEditCommands, public IEditableCursorCommands{};
}
