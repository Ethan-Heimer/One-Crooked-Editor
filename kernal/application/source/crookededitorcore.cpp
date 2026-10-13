#include "crookededitorcore.hpp"
#include "editorvisualstate.hpp"
#include "rendereditorcommand.hpp"

using namespace CrookedEditor;
using namespace CrookedEditor::Application;

CrookedEditorCore::CrookedEditorCore(int argc, char** argv)
    : InputHandler(Application, TerminalController), Renderer(Application, TerminalController),
    TextEditor(Application, TerminalController, ConfigVault){

    Application.OnQuit += {OnCoreQuit, &Event<CrookedEditorCore>::Invoke};

    if(argc >= 2){
       ConfigVault.AddValue("fileName", argv[1]);
    }

    TextEditor.OnEditorCycleComplete += [&](const std::shared_ptr<const TextEditorVisualState> visualData){
        Renderer.AddCommand(Renderer::RenderEditorCommand{TerminalController, visualData});
    };

    TextEditor.SGetNextInputHook = [&]() mutable {
        return InputHandler.GetInput();
    };

    InputHandler.Start();
    Renderer.Start();
    TextEditor.Start();
}

CrookedEditorCore::~CrookedEditorCore(){
    Application.OnQuit.UnsubscribeAll(OnCoreQuit);

    TextEditor.Stop();
    Renderer.Stop();
    InputHandler.Stop();
}

