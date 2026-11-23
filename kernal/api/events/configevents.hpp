#pragma once

#include "configtypes.hpp"
#include "requestbus.hpp"
#include <string>
namespace CrookedEditor::API{
    struct ConfigGetValue : public Events::IRequest<config_union_type, std::string>{};
    struct ConfigAddValue : public Events::IEvent<std::string, config_union_type>{};
}
