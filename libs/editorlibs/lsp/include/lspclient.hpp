#pragma once

#include "ilspnotification.hpp"
#include "ilsprequest.hpp"
#include "ilspresponse.hpp"
#include "lsputils.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <string>
#include <array>
#include <format>
#include <unistd.h>
#include <sys/poll.h>
#include "coroutines.hpp"

namespace LSP{
    using FileDescriptor = int;

    class LSPClient{
        public:
            const int responseTimeout = 3;

            pid_t lspProcessID;
            std::atomic<int> currentRequestID{1};

            std::array<FileDescriptor, 2> parentToChildPipe;
            std::array<FileDescriptor, 2> childToParentPipe;
            std::array<FileDescriptor, 2> childStdErrorPipe;

            std::map<int, Coroutines::LazyAwaitable> schedular;
            std::map<int, std::optional<ResponseJSON>> responseMap;

            std::thread pollThread;
            bool quitPolling;

            LSPClient();
            LSPClient(const LSPClient& other) = delete;
            LSPClient(LSPClient&& other) = delete;

            ~LSPClient();

            void StartLSP(std::string lspName, std::string arguments); 

            void PollReponses(){
                struct pollfd pfds[1];
                pfds[0].fd = childToParentPipe[0];
                pfds[0].events = POLLIN;
                int ret = poll(pfds, 1, 0);

                if(ret == -1){
                    //poll had an error
                }
                else if(!ret){
                    //nothing to read
                }else{
                    std::string responseString = UTILS::GetLSPResponse(childToParentPipe[0]);
                    ResponseJSON responseJson{responseString}; 
                    int responseID = responseJson.responseId;

                    if(responseID > 0){
                        //tell the response await map the response is ready
                        responseMap[responseID]  = std::move(responseJson);
                        schedular[responseID].Resume();    
                    }
                    else{
                        //notification sent
                    }

                    std::fstream debug{"debug.txt", std::ios::out | std::ios::app};
                    debug << std::endl;
                    debug << responseString << std::endl;
                }

            }

            template<typename Req>
            auto SendRequest(Req request) -> Coroutines::Response<decltype(request.GetResponseType())>{
                int id = currentRequestID;
                currentRequestID++;

                std::string requestJson = request.GetJson(id);
                std::string content = std::format("Content-Length: {}\r\n\r\n{}", requestJson.length(), requestJson);
                write(parentToChildPipe[1], content.c_str(), content.length());

                //await response
                schedular[id] = Coroutines::LazyAwaitable{};
                co_await schedular[id];

                // get response from repsonse map
                ResponseJSON resJson = std::move(*responseMap[id]);
                decltype(request.GetResponseType()) response{};

                response.Deserialize(resJson);

                schedular.erase(id);
                responseMap.erase(id);

                co_return std::move(response);
            }

            void SendNotification(const ILSPNotification& notification){
                std::string requestJson = notification.GetJson();
                std::string content = std::format("Content-Length: {}\r\n\r\n{}", requestJson.length(), requestJson);
                write(parentToChildPipe[1], content.c_str(), content.length()); 
            }
    };
}
