#pragma once

#include "filehandling.h"
#include "ieditable.h"
#include <type_traits>

using namespace FileHandling;

namespace Editor::Files {
    template <typename T>
    requires std::is_base_of_v<IEditable, T>
    class IFileHandler : public IFileSaver, public IFileLoader<T>{};
}
