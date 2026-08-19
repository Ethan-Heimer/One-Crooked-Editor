#include "lspclient.hpp"

#include <csignal>
#include <format>
#include <unistd.h>
#include <iostream>
#include "json/json.h"

using namespace LSP;
LSPClient::LSPClient(){
    JInit();
}

LSPClient::~LSPClient(){
    if(lspProcessID > 0){
        kill(lspProcessID, SIGKILL);
    }

    JEnd();
}

pid_t StartLSPProcess(std::string lspName, std::string arguments, 
        std::array<FileDescriptor, 2>& parentToChildPipe, std::array<FileDescriptor, 2>& childToParentPipe, std::array<FileDescriptor, 2>& childStdErrorPipe);

void LSPClient::StartLSP(std::string lspName, std::string arguments){
    lspProcessID = StartLSPProcess(lspName, arguments, parentToChildPipe, childToParentPipe, childStdErrorPipe);
    if(lspProcessID < 0){
        //throw?
    }
}

pid_t StartLSPProcess(std::string lspName, std::string arguments, 
        std::array<FileDescriptor, 2>& parentToChildPipe, std::array<FileDescriptor, 2>& childToParentPipe, std::array<FileDescriptor, 2>& childStdErrorPipe){
    if(pipe(parentToChildPipe.data()) < 0 || pipe(childToParentPipe.data()) < 0 || pipe(childStdErrorPipe.data())){
        return -1;
    }

    pid_t pid = fork();
    if(pid < 0){
        return -1;
    }

    if(pid == 0){
        // in child process
        close(parentToChildPipe[1]); // close write end to child input pipe
                                     
        close(childToParentPipe[0]); // close read end to child output pipe
        close(childStdErrorPipe[0]); // close read end to child output pipe
        
        dup2(parentToChildPipe[0], STDIN_FILENO);  // Redirect stdin to be the input pipe
        dup2(childToParentPipe[1], STDOUT_FILENO); // Redirect stdout to be the output pipe
        dup2(childStdErrorPipe[1], STDERR_FILENO); // Redirect stderr to be the error pipe

        // todo: un hardocde arguments
        execlp(lspName.c_str(), lspName.c_str(), "--log=verbose", "--background-index", NULL);
        exit(0);
    }
    else{
        //parent process
        close(parentToChildPipe[0]); // close read end to parent output pipe
        close(childToParentPipe[1]); // close write end to parent input pipe 
        close(childStdErrorPipe[1]); // close write end to child error pipe 
    }

    return getpid();
}
