#pragma once

#include "json/json.h"
#include <iostream>
#include <string>
#include <string_view>

namespace LSP{
    enum class ResponseCode : int{
        OK = 0,
        InternalError = 1,
        RequestError = 2
    };
    struct ResponseJSON{
        Json* root;
        Json* result;

        int responseId = -1;

        ResponseJSON(std::string_view jsonString){
            this->root = JParseString(jsonString.data());
            JsonValue responseID; 
            JGetValue(root, "id", &responseID);

            if(responseID.hasValue && responseID.isInt){
                responseId = std::stoi(responseID.value->string);
            }

            result = JGetValue(root, "result", NULL);
        }
        ResponseJSON(const ResponseJSON& other) = delete;
        ResponseJSON(ResponseJSON&& other){
            root = other.root;            
            result = other.result;

            responseId = other.responseId;

            other.root = nullptr;
            other.result = nullptr;
            other.responseId = -1;
        }

        ~ResponseJSON(){
            if(root){
                JDelete(&root);
            }
        }

        ResponseJSON& operator=(const ResponseJSON& other) = delete;
        ResponseJSON& operator=(ResponseJSON&& other){
            root = other.root;            
            result = other.result;

            responseId = other.responseId;

            other.root = nullptr;
            other.result = nullptr;
            other.responseId = -1;

            return *this;
        }
    };
    class ILSPResponse{
        public:
            ResponseCode Code{};
            virtual void Deserialize(const ResponseJSON& serverResponse) = 0;
    };
}
