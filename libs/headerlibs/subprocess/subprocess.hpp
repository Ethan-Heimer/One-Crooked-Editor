#pragma once

#include <array>
#include <csignal>
#include <cstdio>
#include <string_view>
#include <sys/wait.h>
#include <type_traits>
#include <unistd.h>
#include <string_view>

namespace Process{
    using FileDescripter = int;
    class Subprocess{
        public: 
            static constexpr int ReadEnd = 0;
            static constexpr int WriteEnd = 1;
    
            pid_t processID = -1;
            std::array<FileDescripter, 2> parentToChildPipe;
            std::array<FileDescripter, 2> childToParentPipe;
            std::array<FileDescripter, 2> childStdErrorPipe;
    
            // this is for the parent process to communicte to the sub process
            FileDescripter input = -1;
            FileDescripter output = -1;
    
            Subprocess(){};
    
            template<typename... Args>
            requires(std::is_same_v<const char*, Args> && ...)
            Subprocess(std::string_view process, const Args&... arguments){
                bool succsess = true;
                succsess = pipe(parentToChildPipe.data()) == 0;
                succsess = pipe(childToParentPipe.data()) == 0;
                succsess = pipe(childStdErrorPipe.data()) == 0;
    
                if(!succsess){
                    return;
                }
    
                processID = fork();
                if(processID < 0)
                    return;
                
                if(bool isChildProcess = processID == 0; isChildProcess){
                    close(parentToChildPipe[WriteEnd]) ; // close write end to child input pipe
                    close(childToParentPipe[ReadEnd]); // close read end to child output pipe
                    close(childStdErrorPipe[ReadEnd]); // close read end to child error pipe
                                                       //
                    dup2(parentToChildPipe[ReadEnd], STDIN_FILENO); // redirect stdin to be the input pipe
                    dup2(childToParentPipe[WriteEnd], STDOUT_FILENO); // redirect stdout to be the output pipe
                    dup2(childStdErrorPipe[WriteEnd], STDERR_FILENO); // redirect stderror to be the error pipe
                                                              
                    execlp(process.data(), process.data(), arguments..., NULL);
                    fflush(stdout);
                    exit(0);
                } else {
                    close(parentToChildPipe[ReadEnd]);
                    close(childToParentPipe[WriteEnd]);
                    close(childStdErrorPipe[WriteEnd]);
    
                    input = parentToChildPipe[WriteEnd];
                    output = childToParentPipe[ReadEnd];
                }
            }
    
            Subprocess(const Subprocess& other) = delete;
            Subprocess(Subprocess&& other){
                parentToChildPipe = std::move(other.parentToChildPipe);
                childToParentPipe = std::move(other.childToParentPipe);
                childStdErrorPipe = std::move(other.childStdErrorPipe);
    
                processID = other.processID;
                input = other.input;
                output = other.output;
    
                other.processID = -1;
                other.input = -1;
                other.output = -1; 
            }
    
            Subprocess& operator=(const Subprocess& other) = delete;
            Subprocess& operator=(Subprocess&& other){
                parentToChildPipe = std::move(other.parentToChildPipe);
                childToParentPipe = std::move(other.childToParentPipe);
                childStdErrorPipe = std::move(other.childStdErrorPipe);
    
                processID = other.processID;
                input = other.input;
                output = other.output;
    
                other.processID = -1;
                other.input = -1;
                other.output = -1; 
    
                return *this;
            }
    
            ~Subprocess(){
                Kill();
            }
    
            void Kill(){
                if(processID == -1)
                    return;
    
                int code = kill(processID, SIGTERM);
                int status;
                waitpid(processID, &status, 0);
            }
    
            bool IsAlive(){
                if(processID == -1)
                    return false; 
    
                int status;
                waitpid(processID, &status, 0);
                return kill(processID, 0) == 0;
            }
    
    };

}
