#include "configvault.hpp"
#include "configtypes.hpp"
#include "configevents.hpp"

#include "requestbus.hpp"
#include "resultbatch.hpp"
#include "runtimeevents.hpp"
#include <expected>
#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>

using namespace CrookedEditor::Runtime;

struct ConfigVault::Impl{
    std::map<std::string, API::config_union_type> vault;
    std::mutex mut;

    Events::RequestBus& requestBus;
    Impl(Events::RequestBus& requestBus) : requestBus(requestBus){
        Events::ResultBatch batch;
        batch = requestBus.AddRequest<API::ConfigAddValue>([&](const std::string& key, const API::config_union_type& value){
            AddValue(key, value);
        });
        
        batch = requestBus.AddRequest<API::ConfigGetValue>([&](const std::string& key) 
                -> API::ConfigGetValue::request_return_type{
            auto value = GetValue(key);
            if(value.has_value()){
                return value.value();
            } else {
                return std::unexpected<std::string>{""};
            }
        });

        batch.Log<API::RuntimeLog>(requestBus);
    }

    ~Impl(){
        std::ignore = requestBus.RemoveRequest<API::ConfigAddValue>();
        std::ignore = requestBus.RemoveRequest<API::ConfigGetValue>();
    }

    void AddValue(const std::string& key, const API::config_union_type& value){
        std::lock_guard<std::mutex> lock{mut};
        vault[key] = value;
    }
           
    API::config_return_type GetValue(std::string_view key){
        std::lock_guard<std::mutex> lock{mut};

        if(!vault.contains(key.data()))
            return std::nullopt;

        std::ofstream t{"tit"};
        auto& val = vault[key.data()];
        t << std::get<std::string>(val);
        return val;
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
