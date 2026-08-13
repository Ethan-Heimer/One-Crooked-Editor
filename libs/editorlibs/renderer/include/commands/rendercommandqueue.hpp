#pragma once

#include "rendercommandtype.hpp"
#include "terminal.hpp"
#include <memory>
namespace Rendering::Commands {
    class RenderingCommandQueue final{
        public:
            RenderingCommandQueue(const Terminal::TerminalController& terminalController);
            ~RenderingCommandQueue();

            template<typename T, typename... U>
            void NewCommand(U... args){
                PushCommand(T{terminalController, args...});
            }

            void PushCommand(RenderCommand command);
            RenderCommand PopCommand();
            bool Empty();
        private:
            const Terminal::TerminalController& terminalController;

            struct Impl;
            std::unique_ptr<Impl> pImpl;

    };
}
