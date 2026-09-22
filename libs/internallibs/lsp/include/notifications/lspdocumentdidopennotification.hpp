#pragma once

#include "ilspnotification.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace LSP{
    class DocumentDidOpenNotification : public ILSPNotification{
        public:
            std::string_view fileName;
            DocumentDidOpenNotification(std::string_view fileName) : fileName(fileName){}

        std::string GetJson() const override{      
            // textdocument/didOpen notification
            std::filesystem::path absolutePath = std::filesystem::canonical(fileName);
            std::ifstream fileContentStream{fileName.data()};
            //make sure file opened
            std::stringstream fileContentBuffer; 

            char ch;
            while(fileContentStream.get(ch)){
                switch(ch){
                    case '\n':
                        fileContentBuffer << "\\n";
                        break;

                    case '\r':
                        fileContentBuffer << "\\r";
                        break;

                    case '"':
                        fileContentBuffer << "\\\"";
                        break;

                    default:
                        fileContentBuffer << ch;
                        break;
                }
            }
       
            // document did open notification
            // * the server needs to keep track of the document in memory
            return 
            "{"
            "  \"jsonrpc\": \"2.0\","
            "  \"method\": \"textDocument/didOpen\","
            "  \"params\": {"
            "       \"textDocument\": {"
            "           \"uri\": \"file:///" + absolutePath.string() + "\","
            "          \"languageId\": \"scss\","
            "          \"version\": 1,"
            "          \"text\": \"" + fileContentBuffer.str() + "\""
            "       }"
            "   }"
            "}";
        }
    };
}
