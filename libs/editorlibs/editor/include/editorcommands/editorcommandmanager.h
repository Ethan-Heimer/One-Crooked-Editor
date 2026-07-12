#pragma once

#include "ieditorcommandmanager.h"

namespace Editor::Commands {
    class EditorCommandManager : public ICommandManager{
        using ICommandManager::ICommandManager;
    };
}
