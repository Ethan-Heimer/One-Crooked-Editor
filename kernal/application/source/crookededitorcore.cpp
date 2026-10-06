#include "crookededitorcore.hpp"
#include "rendereditorcommand.hpp"

using namespace CrookedEditor;
using namespace Renderer;

CrookedEditorCore::CrookedEditorCore(int argc, char** argv)
    : InputHandler(Application, TerminalController), Renderer(Application, TerminalController),
    TextEditor(Application, TerminalController, ConfigVault){

    if(argc < 2){
        return;
    }

    std::string fileName{argv[1]};

    auto renderEditor = [&](const EditorRenderingState renderingState, std::vector<std::string>&& lines,
            std::vector<std::string>&& lineNumbers, int currentLine){
        Renderer.AddCommand(RenderEditorCommand{TerminalController, std::move(renderingState), 
                std::move(lines), std::move(lineNumbers), std::move(currentLine)});
    };

    auto getNextKey = [&]() mutable {
        return InputHandler.GetInput();
    };

    InputHandler.Start();
    Renderer.Start();
    TextEditor.Start(fileName, getNextKey, renderEditor);
}

CrookedEditorCore::~CrookedEditorCore(){
    TextEditor.Stop();
    Renderer.Stop();
    InputHandler.Stop();
}

bool CrookedEditorCore::CoreQuit(){
    return Application.QuitCalled();
}
