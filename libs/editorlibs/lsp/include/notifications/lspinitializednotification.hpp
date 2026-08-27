#pragma once

#include "ilspnotification.hpp"
namespace LSP{
    class InitializedNotification : public ILSPNotification{
        public:
            std::string GetJson() const override{
                return 
                    "{"
                    "  \"jsonrpc\": \"2.0\","
                    "  \"method\": \"initialized\","
                    "  \"params\": {}"
                    "}";
            };
    };
}
