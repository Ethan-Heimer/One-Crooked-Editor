#pragma once

#include <memory>
#include <variant>
#include <string>

namespace CrookedEditor::Application{
    class ConfigVault{
        public:
           ConfigVault();
           ~ConfigVault();

           void AddValue(std::string key, std::variant<const char*, int, bool> value);
        
           template<typename T>
           T FindValueOrDefault(std::string_view key, T);
            
        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
