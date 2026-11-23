#include "configvault.hpp"
#include "configtypes.hpp"
#include "configevents.hpp"

#include "requestbus.hpp"
#include <expected>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <variant>

using namespace CrookedEditor::Application;

struct ConfigVault::Impl{
    std::map<std::string, std::variant<const char*, int, bool>> vault;
    std::mutex mut;

    Impl(Events::RequestBus& requestBus){
        requestBus.AddRequest<API::ConfigAddValue>([&](const std::string& key, const API::config_union_type& value){
            AddValue(key, value);
        });
        
        requestBus.AddRequest<API::ConfigGetValue>([&](const std::string& key) 
                -> API::ConfigGetValue::request_return_type{
            auto value = GetValue(key);
            if(value.has_value()){
                return value.value();
            } else {
                return std::unexpected<std::string>{""};
            }
        });
    }

    void AddValue(const std::string& key, const API::config_union_type& value){
        std::lock_guard<std::mutex> lock{mut};
        vault[key] = std::move(value);
    }
           
    API::config_return_type GetValue(std::string_view key){
        std::lock_guard<std::mutex> lock{mut};

        if(!vault.contains(key.data()))
            return std::nullopt;

        return vault[key.data()];
    }

};
           
ConfigVault::ConfigVault(Events::RequestBus& requestBus) 
    : pImpl(std::make_unique<Impl>(requestBus)){}

ConfigVault::~ConfigVault(){}

void ConfigVault::AddValue(const std::string& key, const API::config_union_type& value){
    pImpl->AddValue(std::move(key), value);
}

CrookedEditor::API::config_return_type ConfigVault::GetValue(const std::string& key){
    return pImpl->GetValue(key);
}
