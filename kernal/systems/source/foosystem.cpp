#include "foosystem.hpp"
#include "systemterminatereasons.hpp"

using namespace CrookedEditor::System;
using namespace CrookedEditor::Application;

SystemStartOptions FooSystem::Start(){
    output = std::ofstream{"foosystem.log"};
    return SystemStartOptions{.SeperateThread = true};
}

bool FooSystem::Update(){
    output << "[foo] This is an update!" << std::endl;

    return false;
}

void FooSystem::Stop(SystemTerminateReasons reason){
    std::string reason_string;

    switch (reason) {
        case Application::SystemTerminateReasons::ShutDown:
            reason_string = "Shut Down";
            break;

        case Application::SystemTerminateReasons::ExecutionEnded:
            reason_string = "Execution Ended";
            break;

        case Application::SystemTerminateReasons::Requested:
            reason_string = "Requested";
            break;

    }

    output << "[Stopping...] " << reason_string << std::endl;;
}
