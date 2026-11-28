#include "inputhandler.hpp"
#include "inputevents.hpp"

#include "resultbatch.hpp"
#include "runtimeevents.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

#include <memory>
#include <tuple>
#include <unistd.h>

#include "inputconstants.hpp"
#include "terminalenviormentevents.hpp"

#include <random>

using namespace CrookedEditor;
using namespace CrookedEditor::Systems;

API::SystemStartOptions InputHandler::Start(){ 
    Events::ResultBatch batch;
    batch = requestBus->SendRequest<API::TermEnterRawMode>();
    batch = requestBus->AddRequest<API::InputCaptured>();
    batch.Log<API::RuntimeLog>(*requestBus);

    return API::SystemStartOptions{.SeperateThread = true};
}

bool InputHandler::Update(){

    int input;
    int bytesRead = read(STDIN_FILENO, &input, 1);

    if(bytesRead > 0)
        std::ignore = requestBus->SendRequest<API::InputCaptured>(input);

    return false;
}

void InputHandler::Stop(API::SystemTerminateReasons reason){
    Events::ResultBatch batch;
    batch = requestBus->RemoveRequest<API::InputCaptured>();
    batch = requestBus->SendRequest<API::TermExitRawMode>();
    batch.Log<API::RuntimeLog>(*requestBus);
}

std::shared_ptr<API::ISystem> InputHandler::Instanciate(API::SystemID SID, 
        Events::RequestBus* requestBus) const{
    return std::make_shared<InputHandler>(SID, requestBus);
}
