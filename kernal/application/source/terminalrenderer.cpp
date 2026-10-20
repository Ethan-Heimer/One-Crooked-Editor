#include "terminalrenderer.hpp"
#include "application.hpp"
#include "rendercommandtype.hpp"
#include "standardrenderercommands.hpp"
#include "terminal.hpp"
#include "tuirenderer.hpp"
#include <chrono>
#include <format>
#include "pingpongringbuffer.hpp"

using namespace CrookedEditor::Application;
using namespace std::chrono;
using namespace Rendering::Commands;
using namespace Terminal;
using namespace Queues;

struct TerminalRenderer::Impl{
    Application& application;
    TerminalController& terminalController;

    ThreadHandle handle{Application::EmpytHandle};

    PingPongRingBuffer<RenderCommand, 65536>  commands;

    Impl(Application& application, TerminalController& terminalController) 
        : application(application), terminalController(terminalController){}

    void Start(){
        handle = application.SpawnThread([&](const std::atomic<bool>& quit){
            Rendering::TUIRenderer renderer{terminalController};

            milliseconds msd {};
            while(!quit){
                milliseconds msb = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

                int row, col{};
                terminalController.GetTerminalSize(row, col);

                if(!commands.IsReadBufferEmpty()){
                    RenderCommand command{};
                    while(commands.Consume(command)){
                        renderer.DoCommand(std::move(command));
                    }

                    auto [vm, rss] = application.MemoryUsage();
                    renderer.DoCommand(RenderString{terminalController, row-1, 0, 
                            std::format("Diagnositcs | MS:{} MEM: {} Mb RSS: {}", msd.count(), vm/1024, rss)});

                    renderer.Display();
                }
                
                commands.SwapBuffers();

                milliseconds msa = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
                msd = msa-msb;
                
                std::this_thread::sleep_for(1ms);
            }

        });

        application.PrintLog(std::format("Rednerer has thread {}", handle));
    }

    void Stop(){
        if(handle != Application::EmpytHandle){
            application.KillThread(handle);
            handle = Application::EmpytHandle;
        }
    }

    void AddCommand(RenderCommand command){
        commands.Produce(std::move(command));
    }
};

TerminalRenderer::TerminalRenderer(Application& application, TerminalController& terminalController) 
    : pImpl(std::make_unique<Impl>(application, terminalController)) {}

TerminalRenderer::~TerminalRenderer(){
    pImpl->Stop();
}

void TerminalRenderer::Start(){
    pImpl->Start();
}

void TerminalRenderer::Stop(){
    pImpl->Stop();
}

void TerminalRenderer::AddCommand(RenderCommand command){
    pImpl->AddCommand(std::move(command));
}
