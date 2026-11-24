#include "runtimecore.hpp"
#include "runtimeevents.hpp"
#include "configvault.hpp"
#include "inputhandler.hpp"
#include "requestbus.hpp"
#include "systemmanager.hpp"
#include "terminalrenderer.hpp"
#include "texteditor.hpp"

#include "texteditor.hpp"
#include <chrono>
#include <thread>

using namespace CrookedEditor;
using namespace Events;

RuntimeCore::RuntimeCore(int argc, char** argv){
    bool quit = false;
    RequestBus.AddRequest<API::RuntimeQuit>([&](){
        quit = true;
    });

    if(argc >= 2){
       ConfigVault.AddValue("fileName", argv[1]);
    }

    SystemManager.StartSystem<Systems::InputHandler>();
    SystemManager.StartSystem<Systems::TerminalRenderer>();
    SystemManager.StartSystem<Systems::TextEditor>();

    while(!quit){
        Application.CleanUp();
        SystemManager.ExecuteSystemQueue();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

RuntimeCore::~RuntimeCore(){
    SystemManager.StopAll();
    Application.Quit();
}

