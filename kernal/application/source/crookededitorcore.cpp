#include "crookededitorcore.hpp"
#include "application.hpp"
#include "applicationevents.hpp"
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

CrookedEditorCore::CrookedEditorCore(int argc, char** argv){
    bool quit = false;
    RequestBus.ListenForEvent<API::ApplicationQuit>([&](){
        quit = true;
    });

    if(argc >= 2){
       ConfigVault.AddValue("fileName", argv[1]);
    }

    SystemManager.StartSystem<Application::InputHandler>();
    SystemManager.StartSystem<Application::TerminalRenderer>();
    SystemManager.StartSystem<Application::TextEditor>();

    while(!quit){
        Application.CleanUp();
        SystemManager.ExecuteSystemQueue();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

CrookedEditorCore::~CrookedEditorCore(){
    SystemManager.StopAll();
}

