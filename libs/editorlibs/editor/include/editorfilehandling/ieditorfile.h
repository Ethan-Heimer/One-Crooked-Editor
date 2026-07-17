#pragma once

#include "filehandling.h"

using namespace FileHandling;

namespace Editor::Files {
    class IFile : public IFileSaveable, public IFileLoadable{};
}
