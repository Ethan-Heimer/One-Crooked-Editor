#include "lsputils.hpp"
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

std::string LSP::UTILS::GetLSPResponse(long fd){
    //grab header
    std::stringstream buffer;
    bool readingHeader = true;
    while(readingHeader){
        char ch;
        int n = read(fd, &ch, 1);
        if(n > 0){
            //marks end of header transmition
            if(ch == '\r'){
                //consume following '\n\r\n'
                read(fd, &ch, 1); 
                read(fd, &ch, 1); 
                read(fd, &ch, 1); 
    
                readingHeader = false;
                }else {
                    buffer << ch;
                }
            }
        }
    
        //get content length
        int contentLength = 0;
        sscanf(buffer.str().c_str(), "Content-Length: %d", &contentLength);
    
        //get the rest of the content
        std::vector<char> contentBuffer;
        contentBuffer.resize(contentLength);
    
        int bytesRead = 0;
        while(bytesRead < contentLength){
            int n = read(fd, &contentBuffer[bytesRead], contentLength - bytesRead);
            bytesRead += n;
        }
    
        return std::string{contentBuffer.begin(), contentBuffer.end()};
}
