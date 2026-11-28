#pragma once

#include "requestbus.hpp"

namespace CrookedEditor::API{
    struct RuntimeLog : public Events::IEvent<std::string>{};
    struct RuntimeQuit : public Events::INotif{};
}
