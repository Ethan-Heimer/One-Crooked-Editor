#include "inputhandler.hpp"
#include "application.hpp"
#include "safequeue.h"
#include "terminal.hpp"
#include <format>
#include <memory>
#include <unistd.h>

#include "inputconstants.hpp"

using namespace CrookedEditor::Application;
using namespace Terminal;

struct InputHandler::Impl{
    Application& application;
    TerminalController& terminalController;

    SafeQueue<int> inputQueue;
    ThreadHandle threadHandle{Application::EmpytHandle};

    Impl(Application& application, TerminalController& terminalController) 
        : application(application), terminalController(terminalController){}

    ~Impl(){
        Stop();
    }

    void Start(){
        terminalController.EnterRawMode();
        threadHandle = application.SpawnThread([&](const std::atomic<bool>& quit) mutable {
            while(!quit){
                int input;
                int bytesRead = read(STDIN_FILENO, &input, 1);

                if(bytesRead > 0)
                    inputQueue.push(input);
            }
        });

        application.PrintLog(std::format("Input Queue has thread {}", threadHandle));
    }

    void Stop(){
        if(threadHandle != Application::EmpytHandle)
            application.KillThread(threadHandle);

        terminalController.ExitRawMode();
    }

    int GetInput(){
        if(inputQueue.empty())
            return Input::NoInput;

        int input = inputQueue.front();
        inputQueue.pop();

        return input;
    }
};

InputHandler::InputHandler(Application& application, TerminalController& terminalController) 
    : pImpl(std::make_unique<Impl>(application, terminalController)){}
InputHandler::~InputHandler(){
    pImpl.reset();
}

void InputHandler::Start(){
    pImpl->Start();
}

void InputHandler::Stop(){
    pImpl->Stop();
}

int InputHandler::GetInput(){
    return pImpl->GetInput();
}
