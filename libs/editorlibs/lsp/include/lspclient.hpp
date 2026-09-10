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
#include <format>
#include <unistd.h>
#include <sys/poll.h>
#include "coroutines.hpp"
#include "functional"

namespace LSP{
    using FileDescriptor = int;

    class LSPClient{
        public:
            const int responseTimeout = 3;

            std::atomic<int> currentRequestID{1};
            std::map<int, Coroutines::LazyAwaitable> schedular;
            std::map<int, std::optional<ResponseJSON>> responseMap;

            int lspProcessHandle;
            int lspInputPipe;
            int lspOutputPipe;

            std::thread pollThread;
            bool quitPolling;

            std::function<void(int handle)> killLspProcess;

            LSPClient(std::function<void(int handle)> killLspProcess);
            LSPClient(const LSPClient& other) = delete;
            LSPClient(LSPClient&& other) = delete;

            ~LSPClient();

            void StartLSP(std::function<std::tuple<int, int, int>()> subprocessGenerator); 

            void PollReponses(){
                struct pollfd pfds[1];
                pfds[0].fd = lspOutputPipe;
                pfds[0].events = POLLIN;
                int ret = poll(pfds, 1, 0);

                if(ret == -1){
                    //poll had an error
                }
                else if(!ret){
                    //nothing to read
                }else{
                    std::string responseString = UTILS::GetLSPResponse(lspOutputPipe);
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
                write(lspInputPipe, content.c_str(), content.length());

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
                write(lspInputPipe, content.c_str(), content.length()); 
            }
    };
}
