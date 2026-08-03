#pragma once

#include "ieditablecursorcommands.h"
#include "ieditableeditcommands.h"
#include "ieditablefilecommands.h"

namespace Editor{
    class IEditable : public IEditableEditCommands, public IEditableCursorCommands, public IEditableFileCommands{}; 
}
