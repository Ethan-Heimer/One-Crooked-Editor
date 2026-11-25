#include "inputhandler.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"
#include <memory>
#include <unistd.h>

#include "inputconstants.hpp"
#include "terminalenviormentevents.hpp"

using namespace CrookedEditor;
using namespace CrookedEditor::Systems;

API::SystemStartOptions InputHandler::Start(){
    requestBus->SendRequest<API::TermEnterRawMode>();
    requestBus->AddRequest<InputCaptured>();

    return API::SystemStartOptions{.SeperateThread = true};
}

bool InputHandler::Update(){
    int input;
    int bytesRead = read(STDIN_FILENO, &input, 1);

    if(bytesRead > 0)
        requestBus->SendRequest<InputCaptured>(input);

    return false;
}

void InputHandler::Stop(API::SystemTerminateReasons reason){
    requestBus->SendRequest<API::TermExitRawMode>();
}

std::shared_ptr<API::ISystem> InputHandler::Instanciate(API::SystemID SID, 
        Events::RequestBus* requestBus) const{
    return std::make_shared<InputHandler>(SID, requestBus);
}
