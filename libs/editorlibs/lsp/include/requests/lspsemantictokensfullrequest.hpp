#pragma once

#include "responses/lspsemantictokensfullresponse.hpp"
#include "ilsprequest.hpp"

#include <filesystem>

namespace LSP{
    class SemanticTokensFullRequest : public ILSPRequest<SemanticTokensFullResponse>{
        public:
            std::string_view filePath;
            SemanticTokensFullRequest(std::string_view filePath)
                : filePath(filePath){}

            std::string GetJson(int requestID) const override{   
                std::filesystem::path absolutePath = std::filesystem::canonical(filePath);
                return
                    "{"
                    "   \"jsonrpc\": \"2.0\","
                    "   \"id\": 2,"
                    "   \"method\": \"textDocument/semanticTokens/full\"," //there is a range version too, probably for when edits are made
                    "   \"params\": {"
                    "       \"textDocument\": {"
                    "       \"uri\": \"file:///" + absolutePath.string() + "\""
                    "       }"
                    "   }"
                    "}";
            }
    };
}
