#pragma once

#include "ilsprequest.hpp"
#include "responses/lspinitilizeresponse.hpp"
#include <string>
#include <unistd.h>

namespace LSP{
    class InitializeRequest : public ILSPRequest<InitializeResponse>{
        public:
            std::string GetJson(int requestID) const override{   
                return "{"
                "  \"jsonrpc\": \"2.0\","
                "  \"id\": " + std::to_string(requestID) +","
                "  \"method\": \"initialize\","
                "  \"params\": {"
                "    \"processId\": " + std::to_string(getpid()) + ","
                "    \"rootUri\": null,"
                "    \"capabilities\": {}"
                "  }"
                "}";
            };
    };
}
