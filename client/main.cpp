#include <functional>

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "configvault.hpp"
#include "terminal.hpp"
#include "terminalrenderer.hpp"
#include "texteditor.hpp"
#include "rendering/include/rendereditorcommand.hpp"

#include "inputhandler.hpp"

#include "application.hpp"

using namespace std;
using namespace std::chrono;

using namespace Editor;

using namespace CrookedEditor::Renderer;

using namespace Rendering;
using namespace Rendering::Commands;

using namespace Terminal;

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }
    string fileName{argv[1]};

    TerminalController terminalController {};

    CrookedEditor::Application::Application app{};
    CrookedEditor::Application::ConfigVault configVault{};

    CrookedEditor::Application::InputHandler inputHandler{app, terminalController};
    CrookedEditor::Application::TerminalRenderer renderer{app, terminalController};

    CrookedEditor::Application::TextEditor textEditor{app, terminalController, configVault};

    auto renderEditor = [&](const EditorRenderingState renderingState, std::vector<std::string>&& lines,
            std::vector<std::string>&& lineNumbers, int currentLine){
        renderer.AddCommand(RenderEditorCommand{terminalController, std::move(renderingState), std::move(lines), std::move(lineNumbers), std::move(currentLine)});
    };

    auto getNextKey = [&]() mutable {
        return inputHandler.GetInput();
    };

    inputHandler.Start();
    renderer.Start();
    textEditor.Start(fileName, getNextKey, renderEditor);

    app.Run();

    textEditor.Stop();
    renderer.Stop();
    inputHandler.Stop();

    return 0;
}
