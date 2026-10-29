#include "foosystem.hpp"
#include "systemterminatereasons.hpp"
#include <format>

using namespace CrookedEditor::System;
using namespace CrookedEditor::Application;
using namespace CrookedEditor::Application::SystemHandling;

SystemStartOptions FooSystem::Start(){
    return SystemStartOptions{.SeperateThread = true};
}

bool FooSystem::Update(){
    application.PrintLog("[foo] this is a message from foo");
    return false;
}

void FooSystem::Stop(SystemTerminateReasons reason){
    std::string reason_string;

    switch (reason) {
        case SystemTerminateReasons::ShutDown:
            reason_string = "Shut Down";
            break;

        case SystemTerminateReasons::ExecutionEnded:
            reason_string = "Execution Ended";
            break;

        case SystemTerminateReasons::Requested:
            reason_string = "Requested";
            break;

    }

    application.PrintLog(std::format("foo stopping due to {}", reason_string));
}
