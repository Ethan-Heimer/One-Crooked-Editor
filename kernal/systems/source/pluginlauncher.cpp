#include "pluginlauncher.hpp"
#include <fstream>
#include <string_view>

#include <filesystem>
#include <dlfcn.h>

void CrookedEditor::Application::PluginLauncher::LaunchPluginsIn(std::string_view path){
    for(const auto& file : std::filesystem::directory_iterator(path)){    
        if(bool isPlugin = file.path().extension() == ".so" || file.path().extension() == ".dylib"; isPlugin){
            void* plugin = dlopen(file.path().string().c_str(), RTLD_LAZY);
            if(!plugin)
                continue;

            void* entry = dlsym(plugin, "start");
            if(!entry)
                continue;

            dlerror();

            void(*func)() = reinterpret_cast<void(*)()>(entry);
            func();

            dlclose(plugin);
        }
    }
}
