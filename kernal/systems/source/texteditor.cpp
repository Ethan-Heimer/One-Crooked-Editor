#include "texteditor.hpp"
#include "application.hpp"
#include "bufferfilehandler.h"
#include "configvault.hpp"

#include "editorcontext.h"
#include "editorstateparameters.h"
#include "editorstates.h"
#include "editorvisualstate.hpp"
#include "terminal.hpp"

#include "inputconstants.hpp"
#include "editorvisualstatehelper.hpp"

#include <format>
#include <memory>
#include <string>
#include <thread>

using namespace CrookedEditor::Application;
using namespace CrookedEditor::Buffers;
using namespace CrookedEditor::States;

using namespace Editor;
using namespace Editor::States;

using namespace Terminal;
using namespace CrookedEditor::Components;

struct TextEditor::Impl{
    Application& app;
    ConfigVault& config;
    TerminalController& terminalController;

    CycleCompleteEventType& onCycleCompleteEvent;
    InputHookType& getNextKey;

    ThreadHandle handle{Application::EmpytHandle};

    Impl(Application& app, TerminalController& terminalController, 
         ConfigVault& config, CycleCompleteEventType& onCycleCompleteEvent,
         InputHookType& getNextKey) 
        : app(app), terminalController(terminalController), config(config), 
          onCycleCompleteEvent(onCycleCompleteEvent), getNextKey(getNextKey){} 

    void Start(){
        if(handle >= 0){
            Stop();
        }

        auto editorJob = [this](const std::atomic<bool>& quitToken){
            std::string fileName = config.FindValueOrDefault("fileName", "newfile.txt");

            EditorContext context{BufferFileInterpreter{}, 
                DefaultStates<NormalState, InsertState>{}, fileName};

            stringstream inputStream;

            /* constants for figureing out rendering data, move later */
            const int lineColumnSpace = 4;
            const int verticalScrollThreshold = 5;
            const int invisableRows = 2; // -- offset of how meny rows should
                                         // not be rendered at the bottom
                                         
            TextEditorVisualState visualState;
            EditorVisualStateHelper visualStateHelper{}; 

            while(!quitToken){
                int key;

                do{
                    key = getNextKey();
                    if(key != Input::NoInput)
                        inputStream << static_cast<char>(key);

                } while (key != Input::NoInput);

                context.Update(std::move(inputStream).str());
                inputStream.str("");
                inputStream.clear();

                if(context.quit){
                    app.Quit();
                }

                visualStateHelper.UpdateVisualState(terminalController, 
                        context, visualState, invisableRows, verticalScrollThreshold, 
                        lineColumnSpace);
        
                auto temp = visualState.Dispatch();
                    auto visualStatePtr = std::make_shared<const TextEditorVisualState>(std::move(temp));
                onCycleCompleteEvent.Invoke(visualStatePtr);
                std::this_thread::sleep_for(1ms);
            };

        }; 

        handle = app.SpawnThread(editorJob);
        app.PrintLog(std::format("Text Editor has thread id {}", handle));
    }
    
    void Stop(){
        if(handle < 0)
            return; 

        app.KillThread(handle);
        handle = Application::EmpytHandle; 
    }
};


TextEditor::TextEditor(Application& app, TerminalController& terminalController,
        ConfigVault& config) : pImpl(std::make_unique<Impl>(app, terminalController, 
                config, OnEditorCycleComplete, SGetNextInputHook)){}

TextEditor::~TextEditor(){
    Stop();
}

void TextEditor::Start(){
    pImpl->Start();
}

void TextEditor::Stop(){
    pImpl->Stop();
}
