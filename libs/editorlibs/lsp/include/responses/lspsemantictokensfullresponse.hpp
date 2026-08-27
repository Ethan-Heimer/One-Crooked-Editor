#pragma once

#include "ilspresponse.hpp"
#include "utils/token.h"
#include "json/json.h"
#include <vector>

namespace LSP{
    class SemanticTokensFullResponse : public ILSPResponse{
        public:
        std::vector<int> data;
            virtual void Deserialize(const ResponseJSON& serverResponse) override{
                Json* dataObject = JGetValue(serverResponse.result, "data", NULL);
                if(!dataObject)
                    return;

                for(int i = 0; i < dataObject->childLength; i++){
                    Json* child = dataObject->children[i];
                    
                    if(child->valueType == INT_VALUE){
                        data.push_back(std::stoi(child->data->string));
                    }
                }
            }
    };
}
