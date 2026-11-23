#pragma once

#include <memory>
#include <string>

#include "configtypes.hpp"
#include "requestbus.hpp"

namespace CrookedEditor::Application{
    class ConfigVault final{
        public:
            ConfigVault(Events::RequestBus& requestBus);
            ~ConfigVault();

            void AddValue(const std::string& key, const API::config_union_type& value);
            API::config_return_type GetValue(const std::string& key);
            
        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
