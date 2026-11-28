#pragma once
#include "requestbus.hpp"

namespace CrookedEditor::API{
    struct InputCaptured : public Events::IEvent<int>{};
}
