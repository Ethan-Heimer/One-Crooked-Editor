#include "crookededitorcore.hpp"
#include "application.hpp"
#include "editorvisualstate.hpp"
#include "foosystem.hpp"
#include "pluginlauncher.hpp"
#include "rendereditorcommand.hpp"
#include "systemmanager.hpp"

#include <chrono>
#include <thread>

using namespace CrookedEditor;
using namespace CrookedEditor::Components;
using namespace Events;

CrookedEditorCore::CrookedEditorCore(int argc, char** argv)
    : InputHandler(Application, TerminalController), Renderer(Application, TerminalController),
    TextEditor(Application, TerminalController, ConfigVault), pluginLauncher(), SystemManager(Application, ConfigVault, eventBus){

    bool quit = false;
    Application.OnQuit += {OnCoreQuit, &Event<CrookedEditorCore>::Invoke};
    OnCoreQuit += [&](){
       quit = true; 
    };

    if(argc >= 2){
       ConfigVault.AddValue("fileName", argv[1]);
    }

    TextEditor.OnEditorCycleComplete += [&](const std::shared_ptr<const TextEditorVisualState> visualData){
        Renderer.AddCommand(Renderer::RenderEditorCommand{TerminalController, visualData});
    };

    TextEditor.SGetNextInputHook = [&]() mutable {
        return InputHandler.GetInput();
    };

    //pluginLauncher.LaunchPluginsIn("plugins");

    InputHandler.Start();
    Renderer.Start();
    TextEditor.Start();

    SystemManager.StartSystem<System::FooSystem>();

    while(!quit){
        Application.CleanUp();
        SystemManager.ExecuteSystemQueue();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

CrookedEditorCore::~CrookedEditorCore(){
    SystemManager.StopAll();

    TextEditor.Stop();
    Renderer.Stop();
    InputHandler.Stop();

    Application.OnQuit.UnsubscribeAll(OnCoreQuit);

}

