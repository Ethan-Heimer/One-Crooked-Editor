#include "terminalrenderer.hpp"
#include "isystem.hpp"
#include "rendercommandconstructor.hpp"
#include "rendercommandtype.hpp"
#include "requestbus.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"
#include "terminalinterface.hpp"
#include "tuirenderer.hpp"
#include "pingpongringbuffer.hpp"
#include "terminalinterfacehelper.hpp"

using namespace CrookedEditor::Systems;
using namespace CrookedEditor::Renderer;
using namespace Rendering::Commands;
using namespace Queues;
using namespace Rendering;

struct TerminalRenderer::Impl{
    API::SystemID SID;
    Events::RequestBus* requestBus;

    TerminalInterface terminalInterface;
    Rendering::TUIRenderer renderer;

    PingPongRingBuffer<RenderCommand, 65536>  commands;

    Impl(API::SystemID SID, Events::RequestBus* requestBus)
    : requestBus(requestBus){
        terminalInterface = MakeTerminalInterface(*requestBus);
    }

    API::SystemStartOptions Start(){
        requestBus->AddRequest<PublishRenderCommand>([&]
                (std::shared_ptr<IRenderCommandConstructor> commandConstructor){
            commands.Produce(commandConstructor->Construct());
        });

        renderer.Start(terminalInterface);

        return API::SystemStartOptions{.SeperateThread = true};
    }

    bool Update(){
        commands.SwapBuffers();
        if(!commands.IsReadBufferEmpty()){
            RenderCommand command{};
            while(commands.Consume(command)){
                renderer.DoCommand(std::move(command));
            }

            renderer.Display(terminalInterface);
        }
                
        return false;
    };

    void Stop(API::SystemTerminateReasons reasons){
        renderer.Start(terminalInterface);
    }
};

TerminalRenderer::TerminalRenderer() : API::ISystem(){};
TerminalRenderer::TerminalRenderer(API::SystemID SID, Events::RequestBus* requestBus)
    : ISystem(SID, requestBus),
    pImpl(std::make_unique<Impl>(SID, requestBus)) {}

TerminalRenderer::~TerminalRenderer(){};

CrookedEditor::API::SystemStartOptions TerminalRenderer::Start(){
    return pImpl->Start();
}

bool TerminalRenderer::Update(){
    return pImpl->Update();
}

void TerminalRenderer::Stop(CrookedEditor::API::SystemTerminateReasons reason){
    pImpl->Stop(reason);
}

std::shared_ptr<CrookedEditor::API::ISystem> TerminalRenderer::Instanciate(CrookedEditor::API::SystemID SID, 
        Events::RequestBus* requestBus) const{
    return std::make_shared<TerminalRenderer>(SID, requestBus);
}
