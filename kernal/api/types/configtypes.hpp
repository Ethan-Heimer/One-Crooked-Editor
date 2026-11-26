#pragma once

#include <optional>
#include <variant>

namespace CrookedEditor::API{
    using config_union_type = std::variant<const char*, int, bool>;
    using config_return_type = std::optional<config_union_type>;
}
