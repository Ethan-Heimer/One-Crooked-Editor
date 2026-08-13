#include "commands/rendercommandqueue.hpp"
#include "rendercommandtype.hpp"
#include "safequeue.h"
#include "terminal.hpp"
#include <memory>

using namespace Rendering::Commands;
using namespace Terminal;

struct RenderingCommandQueue::Impl{
    void PushCommand(RenderCommand command){
        commands.move(std::move(command));
    }

    RenderCommand PopCommand(){
        RenderCommand command = std::move(commands.front());
        commands.pop();
        return command;
    }

    bool Empty(){
        return commands.empty();
    }
   
    SafeQueue<RenderCommand> commands;
};

RenderingCommandQueue::RenderingCommandQueue(const TerminalController& terminalController) 
    : terminalController(terminalController), pImpl(std::make_unique<Impl>()){}
RenderingCommandQueue::~RenderingCommandQueue() = default;

void RenderingCommandQueue::PushCommand(RenderCommand command){
    pImpl->PushCommand(std::move(command));
}

RenderCommand RenderingCommandQueue::PopCommand(){
    return pImpl->PopCommand();
}

bool RenderingCommandQueue::Empty(){
    return pImpl->Empty();
}
