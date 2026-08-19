#pragma once

#include "ilspresponse.hpp"
#include "utils/token.h"
#include "json/json.h"
#include <cstddef>
#include <cstring>
#include <format>
#include <map>
#include <variant>

namespace LSP{
    class InitializeResponse final : public ILSPResponse{
        public:
            std::map<std::string, std::variant<int, float, bool, std::string>> capabilities{};
            virtual void Deserialize(const ResponseJSON& serverResponse) override{
                Json* capabilitiesOBJ = JGetValue(serverResponse.result, "capabilities", NULL);
                if(!capabilitiesOBJ){
                    return;
                }

                AddCapabilities("", capabilitiesOBJ);
            }

        private:
            void AddCapabilities(std::string supname, Json* object){
                if(object == nullptr){
                    return;
                }

                for(int i = 0; i < object->childLength; i++){
                    Json* child = object->children[i];

                    std::string key = std::format("{}{}", supname, child->key->string);
                    if(child->type == OBJECT || child->type == ARRAY){
                        AddCapabilities(std::format("{}{}.", supname, key), child);
                    }
                    else if(child->valueType == BOOL_VALUE){
                        const char* boolValue = child->data->string;
                        if(strcmp(boolValue, "true") == 0)
                            this->capabilities[key] = true;
                        else
                            this->capabilities[key] = false;
                    }
                    else if(child->valueType == FLOAT_VALUE){
                        this->capabilities[key] = std::stof(child->data->string);
                    }

                    else if(child->valueType == INT_VALUE){
                        this->capabilities[key] = std::stoi(child->data->string);
                    }
                    else if(child->valueType == STRING_VALUE){
                        this->capabilities[key] = child->data->string;
                    }
                }
            }
    };
}
