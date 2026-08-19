#pragma once

#include "ilspresponse.hpp"
#include "lsputils.hpp"
#include <condition_variable>
#include <fstream>
#include <future>
#include <ios>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <array>
#include <format>
#include <thread>
#include <unistd.h>
#include <sys/poll.h>
#include <variant>
#include <chrono>

namespace LSP{
    using FileDescriptor = int;

    struct AwaitResponseInfo{
        std::mutex mutex{};
        std::condition_variable condition{};
        bool recivedResponse = false;
    };

    class LSPClient{
        public:
            const int responseTimeout = 3;

            pid_t lspProcessID;
            int currentRequestID{1};

            std::mutex currentIdMut;

            std::array<FileDescriptor, 2> parentToChildPipe;
            std::array<FileDescriptor, 2> childToParentPipe;
            std::array<FileDescriptor, 2> childStdErrorPipe;

            //std::monstate represents the absence of a response in this case
            std::map<int, std::shared_ptr<AwaitResponseInfo>> awaitResponseMap;
            std::map<int, std::variant<std::monostate, ResponseJSON>> responseMap;

            LSPClient();
            ~LSPClient();
            LSPClient(const LSPClient& other) = delete;
            LSPClient(LSPClient&& other) = default;

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
                    std::cout << responseID;

                    if(responseID > 0){
                        //tell the response await map the response is ready
                        std::shared_ptr<AwaitResponseInfo> awaitResponseInfo = awaitResponseMap[responseID];
                        std::lock_guard lock{awaitResponseInfo->mutex};
                        
                        awaitResponseInfo->recivedResponse = true;
                        responseMap[responseID]  = std::move(responseJson);
                        awaitResponseInfo->condition.notify_one();
                    }
                    else{
                        //notification sent
                    }

                    //std::fstream ostream{"debug.txt", std::ios::out};
                    //ostream << responseString;
                    //read
                }

            }

            int SendRequest(){
                /*
                currentIdMut.lock();
                int id = currentRequestID;
                currentRequestID++;
                currentIdMut.unlock();

                std::string requestJson = request.GetJson(id);

                std::string content = std::format("Content-Length: {}\r\n\r\n{}", requestJson.length(), requestJson);
                write(parentToChildPipe[1], content.c_str(), content.length());

                std::shared_ptr<AwaitResponseInfo> responseInfo = std::make_shared<AwaitResponseInfo>();
                awaitResponseMap[id] = responseInfo;

                //await for the main poller to get response from lsp server
                std::unique_lock lock{responseInfo->mutex};
                responseInfo->condition.wait(lock, [responseInfo]{return responseInfo->recivedResponse;});

                ResponseJSON resJson = std::get<ResponseJSON>(std::move(responseMap[id]));
                Res response{};
                response.Deserialize(resJson);

                awaitResponseMap.erase(id);
                responseMap.erase(id);
                */

                return 10;
            }

            template<typename Res, typename Req>
            auto SendRequestAsync(Req request){
                return std::async(std::launch::async, &LSPClient::SendRequest, this);
            }
    };
}
