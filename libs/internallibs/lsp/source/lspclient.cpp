#include "lspclient.hpp"

#include <chrono>
#include <csignal>
#include <functional>
#include <thread>
#include <unistd.h>
#include "json/json.h"

using namespace LSP;
LSPClient::LSPClient(std::function<void(int)> killLspProcess) : killLspProcess(killLspProcess){
    JInit();
}

LSPClient::~LSPClient(){
    quitPolling = true;

    if(pollThread.joinable())
        pollThread.join();
    
    killLspProcess(lspProcessHandle);

    JEnd();
}

void LSPClient::StartLSP(std::function<std::tuple<int, int, int>()> subprocessGenerator){
    auto [lspProcessHandle, lspInputPipe, lspOutputPipe] = subprocessGenerator();

    std::cout << "input " << lspInputPipe << " Lsp Output " << lspOutputPipe << std::endl;

    pollThread = std::thread{[this](){
        while(!quitPolling){
            PollReponses();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        };
    }};
}
