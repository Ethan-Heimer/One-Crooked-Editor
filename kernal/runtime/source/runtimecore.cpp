#include "runtimecore.hpp"
#include "runtimeevents.hpp"
#include "inputhandler.hpp"
#include "terminalrenderer.hpp"
#include "texteditor.hpp"

#include <algorithm>
#include <fstream>

using namespace CrookedEditor;
using namespace Events;

inline void ParseArgs(int argc, char** argv, Runtime::ConfigVault& vault);

RuntimeCore::RuntimeCore(int argc, char** argv){
    bool quit = false;
    std::ofstream log{"debug.log"};

    ParseArgs(argc, argv, ConfigVault);

    std::ignore = RequestBus.AddRequest<API::RuntimeQuit>([&](){
        quit = true;
    });

    std::ignore = RequestBus.AddRequest<API::RuntimeLog>([&](const std::string& message){
        log << message << std::endl;
    });

    SystemManager.StartSystem<Systems::InputHandler>();
    SystemManager.StartSystem<Systems::TerminalRenderer>();
    SystemManager.StartSystem<Systems::TextEditor>();

    while(!quit){
        ResourceManager.CleanUp();
        SystemManager.ExecuteSystemQueue();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    SystemManager.StopAll();
    ResourceManager.Quit();

    std::ignore = RequestBus.RemoveRequest<API::RuntimeLog>();
    std::ignore = RequestBus.RemoveRequest<API::RuntimeQuit>();
}

inline void ParseArgs(int argc, char** argv, Runtime::ConfigVault& vault){
    for(int i = 0; i < argc; i++){
        std::stringstream stream{argv[i]};

        std::string key, value;

        std::getline(stream, key, '=');
        std::getline(stream, value);
        
        if(value.empty()) 
            continue;

        if(std::all_of(value.begin(), value.end(), [](auto& c){
            return std::isdigit(c);
        })){
            int number = std::stoi(value);
            vault.AddValue(key, number);
        }else if(value == "true"){
            vault.AddValue(key, true);
        } else if(value == "false"){
            vault.AddValue(key, false);
        } else {
            vault.AddValue(key, value.c_str());
        }
    }
}
