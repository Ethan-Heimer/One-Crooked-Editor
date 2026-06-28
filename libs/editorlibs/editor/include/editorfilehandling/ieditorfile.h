#pragma once

#include "filehandling.h"

using namespace FileHandling;

namespace Editor::Files {
    class IEditorFile : public IFileSaveable, public IFileLoadable{};
}
