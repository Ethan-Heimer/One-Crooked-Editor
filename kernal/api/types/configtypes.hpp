#pragma once

#include <optional>
#include <string>
#include <variant>

namespace CrookedEditor::API{
    using config_union_type = std::variant<std::string, int, bool>;
    using config_return_type = std::optional<config_union_type>;
}
