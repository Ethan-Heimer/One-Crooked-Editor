#include "inputhandler.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"
#include <unistd.h>

#include "inputconstants.hpp"
#include "terminalenviormentevents.hpp"

using namespace CrookedEditor::Application;
using namespace CrookedEditor::Application::SystemHandling;

SystemStartOptions InputHandler::Start(){
    // EnterRawMode event
    requestBus.SendRequest<API::TermEnterRawMode>();
    requestBus.AddRequest<InputCaptured>();

    return SystemStartOptions{.SeperateThread = true};
}

bool InputHandler::Update(){
    int input;
    int bytesRead = read(STDIN_FILENO, &input, 1);

    if(bytesRead > 0)
        requestBus.SendRequest<InputCaptured>(input);

    return false;
}

void InputHandler::Stop(SystemTerminateReasons reason){
    // ExitRawMode event
    requestBus.SendRequest<API::TermExitRawMode>();
}
