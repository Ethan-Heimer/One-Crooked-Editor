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

#include <format>
#include <memory>
#include <string>
#include <vector>

using namespace CrookedEditor::Application;
using namespace CrookedEditor::Buffers;
using namespace CrookedEditor::States;

using namespace Editor;
using namespace Editor::States;

using namespace Terminal;

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
            const int verticalScrollThreshhold = 5;
            const int invisableRows = 2; // -- offset of how meny rows should
                                         // not be rendered at the bottom
            TextEditorVisualState visualState;

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

                /* calculate visual offsets and send it off to the renderer */
                int row, col;
                terminalController.GetTerminalSize(row, col);

                visualState.cursorCol = context.buffer->GetCursorX();

                int currentLineNumber = context.buffer->GetCurrentLineNumber();
                int visableLines = row - invisableRows;

                ScrollViewVertical(verticalScrollThreshhold, visableLines, currentLineNumber, visualState);
                ScrollViewHorizontal(6, col, visualState);

                int currentLine = context.buffer->GetCurrentLineNumber();

                GetDataInView(visableLines, visualState, *context.buffer);
                CalculateLineNumberColumnValues(visualState, lineColumnSpace);

                visualState.lastRowNumber = currentLineNumber;
                
                auto temp = visualState.Dispatch();
                auto visualStatePtr = std::make_shared<const TextEditorVisualState>(std::move(temp));

                onCycleCompleteEvent.Invoke(visualStatePtr);
                app.PrintLog(fileName);
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

    /* View Calculations for rendering */
    /* I handle the persistent visual state stuff here as 
     * 1. handling it in a rendering command (the old way) can cause a segfault
     *    as the struct holding the state can be deallocated before the command 
     *    finished (multi threading things - dumb of me for passing a resource to
     *    another thread like that)
     * 2. a 'rendering command' should not do any computation outside of "hey, 
     *    how do i display the data you gave me". figuring out that 'data' should 
     *    not be its job.
     */ 

    void ScrollViewVertical(const int& verticalScrollThreshold, const int& visableLines, const int& currentLineNumber, TextEditorVisualState& visualState){
        /*
         * The offset is kinda backwards as its relative to the current line and not the start of the buffer.
         * Meaning that if the view needs to stay, the offset should change to counteract the difference in position of the new line.
         */
    
        int lineDelta = currentLineNumber - visualState.lastRowNumber;
        if(lineDelta > 0){
            int linesFromBottom = visableLines - (currentLineNumber - (currentLineNumber - visualState.currentLineViewOffset));
    
            for(int i = lineDelta; i > 0; i--){
                if(linesFromBottom - i >= verticalScrollThreshold)
                    visualState.currentLineViewOffset++;
                else
                    break;
            }
        }
        else if(lineDelta < 0){
            int linesFromTop = currentLineNumber - (currentLineNumber - visualState.currentLineViewOffset);
    
            for(int i = 0; i > lineDelta; i--){
                if(linesFromTop + i >= verticalScrollThreshold)
                    visualState.currentLineViewOffset--;
    
                else if(currentLineNumber <= verticalScrollThreshold){
                    visualState.currentLineViewOffset = currentLineNumber;
                } 
                else
                    break;
            }
        }
    
        visualState.firstLineNumberShown = currentLineNumber - visualState.currentLineViewOffset;
        visualState.lastLineNumberShown = visableLines + visualState.firstLineNumberShown; 
    }
    
    void ScrollViewHorizontal(const int& lineNumberColumnWidth, const int& termCol, TextEditorVisualState& visualState){
        while(visualState.cursorCol > termCol + visualState.colOffset - lineNumberColumnWidth){
            visualState.colOffset++;
        }
    
        while(visualState.cursorCol < visualState.colOffset){
            visualState.colOffset--; 
        }
    }

    void GetDataInView(const int& visableLines, TextEditorVisualState& visualState, const IEditable& buffer){
        visualState.lines.reserve(visableLines);
        visualState.lines.reserve(visableLines);
    
        auto start = buffer.BeginStepsFromCurrentLine(-visualState.currentLineViewOffset);
        auto end = buffer.EndStepsFromCurrentLine((visableLines-visualState.currentLineViewOffset));

        for(auto line = start; line != end; ++line){
            visualState.lines.push_back(*line);
            visualState.lineNumbers.push_back(std::to_string(line.LineNumber()));
        }

    }

    void CalculateLineNumberColumnValues(TextEditorVisualState& visualState, const int& minLineNumberColWidth){
        int lineNumberLength{};
        int maxLineNumberLength{};
    
        for(size_t i = 0; i < visualState.lineNumbers.size(); i++){
            lineNumberLength = visualState.lineNumbers[i].length();
            if(maxLineNumberLength < lineNumberLength)
                maxLineNumberLength = lineNumberLength;
        }
    
        visualState.lineNumberPaddingWidth = minLineNumberColWidth - maxLineNumberLength;
        if(visualState.lineNumberPaddingWidth < 0)
            visualState.lineNumberPaddingWidth *= -1;
        
        visualState.lineNumberColumnWidth = visualState.lineNumberPaddingWidth + maxLineNumberLength;
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
