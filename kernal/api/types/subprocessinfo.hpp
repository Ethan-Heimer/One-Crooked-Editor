#pragma once

#include "subprocess.hpp"
#include "applicationtypes.hpp"

namespace CrookedEditor::API{
    struct SubprocessInfo{
        SubprocessHandle subprocessHandle;   

        Process::FileDescripter inputFd;
        Process::FileDescripter outputFd;
    };
}
