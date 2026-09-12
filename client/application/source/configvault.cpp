#include "configvault.hpp"
#include <map>
#include <memory>
#include <variant>

using namespace CrookedEditor::Application;

struct ConfigVault::Impl{
    std::map<std::string, std::variant<const char*, int, bool>> vault;

    void AddValue(std::string key, std::variant<const char*, int, bool> value){
        vault[key] = std::move(value);
    }

    template<typename T>
    T FindValueOrDefault(std::string_view key, T fallback){
        if(!vault.contains(key.data()))
            return fallback;

        if(std::holds_alternative<T>(vault.at(key.data())))
            return std::get<T>(vault[key.data()]);
        else
         return fallback;
    }

};
           

ConfigVault::ConfigVault() : pImpl(std::make_unique<Impl>()){}
ConfigVault::~ConfigVault(){}

void ConfigVault::AddValue(std::string key, std::variant<const char*, int, bool> value){
    pImpl->AddValue(std::move(key), value);
}

template<typename T>
T ConfigVault::FindValueOrDefault(std::string_view key, T fallback){
    return pImpl->FindValueOrDefault(key, fallback);
}

template const char* ConfigVault::FindValueOrDefault<const char*>(std::string_view key, const char* fallback);
template int ConfigVault::FindValueOrDefault<int>(std::string_view key, int fallback);
template bool ConfigVault::FindValueOrDefault<bool>(std::string_view key, bool fallback);
