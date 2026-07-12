#pragma once

#include "ieditorcommandmanager.h"

namespace Editor::Commands {
    class EditorCommandManager : public IEditorCommandManager{
        using IEditorCommandManager::IEditorCommandManager;
    };
}
