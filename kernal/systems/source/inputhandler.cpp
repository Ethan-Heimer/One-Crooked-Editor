#include "inputhandler.hpp"
#include "application.hpp"
#include "terminal.hpp"
#include <format>
#include <memory>
#include <unistd.h>
#include "ringbuffer.hpp"

#include "inputconstants.hpp"

using namespace CrookedEditor::Application;
using namespace Terminal;
using namespace Queues;

struct InputHandler::Impl{
    Application& application;
    TerminalController& terminalController;

    RingBuffer<int, 65536> inputQueue;
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
                    inputQueue.Push(input);
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
        int input = Input::NoInput;
        inputQueue.Pop(input);

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
