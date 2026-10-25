#pragma once

#include <string_view>

namespace  CrookedEditor::Application{
    class PluginLauncher{
        public:
            void LaunchPluginsIn(std::string_view path);
    };
}
