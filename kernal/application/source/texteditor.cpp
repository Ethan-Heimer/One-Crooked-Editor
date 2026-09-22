#include "texteditor.hpp"
#include "application.hpp"
#include "bufferfilehandler.h"
#include "configvault.hpp"

#include "editorcontext.h"
#include "editorrenderingstate.hpp"
#include "editorstateparameters.h"
#include "editorstates.h"
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

using namespace CrookedEditor::Renderer;
using namespace Terminal;

struct TextEditor::Impl{
    Application& app;
    ConfigVault& config;
    TerminalController& terminalController;

    ThreadHandle handle{Application::EmpytHandle};

    Impl(Application& app, TerminalController& terminalController, ConfigVault& config) 
        : app(app), terminalController(terminalController), config(config){} 

    void Start(std::string_view fileName, std::function<int()> getNextKey, 
            std::function<void(EditorRenderingState renderingState, std::vector<std::string>&& lines,
                std::vector<std::string>&& lineNumbers, int isCurrentLine)> renderEditor){

        if(handle >= 0){
            Stop();
        }

        auto editorJob = [this, fileName, getNextKey, renderEditor](const std::atomic<bool>& quitToken){
            EditorContext context{BufferFileInterpreter{}, DefaultStates<NormalState, InsertState>{}, fileName.data()};
            stringstream inputStream;

            EditorRenderingState renderingState{};

            /* constants for figureing out rendering data, move later */
            const int lineColumnSpace = 4;
            const int verticalScrollThreshhold = 5;
            const int invisableRows = 2; // -- offset of how meny rows should
                                         // not be rendered at the bottom
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

                renderingState.cursorCol = context.buffer->GetCursorX();

                int currentLineNumber = context.buffer->GetCurrentLineNumber();
                int visableLines = row - invisableRows;

                ScrollViewVertical(verticalScrollThreshhold, visableLines, currentLineNumber, renderingState);
                ScrollViewHorizontal(6, col, renderingState.cursorCol, renderingState.colOffset);

                std::vector<std::string> lines;
                std::vector<std::string> lineNumbers;

                int currentLine = context.buffer->GetCurrentLineNumber();

                GetDataInView(visableLines, renderingState, *context.buffer, 
                        lines, lineNumbers);


                CalculateLineNumberColumnValues(lineNumbers, lineColumnSpace, 
                        renderingState.lineNumberColumnWidth, renderingState.lineNumberPaddingWidth);

                renderEditor(renderingState, std::move(lines), std::move(lineNumbers), currentLine);

                renderingState.lastRowNumber = currentLineNumber;
                //
                
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

    void ScrollViewVertical(const int& verticalScrollThreshold, const int& visableLines, const int& currentLineNumber, EditorRenderingState& renderingData){
        /*
         * The offset is kinda backwards as its relative to the current line and not the start of the buffer.
         * Meaning that if the view needs to stay, the offset should change to counteract the difference in position of the new line.
         */
    
        int lineDelta = currentLineNumber - renderingData.lastRowNumber;
        if(lineDelta > 0){
            int linesFromBottom = visableLines - (currentLineNumber - (currentLineNumber - renderingData.currentLineViewOffset));
    
            for(int i = lineDelta; i > 0; i--){
                if(linesFromBottom - i >= verticalScrollThreshold)
                    renderingData.currentLineViewOffset++;
                else
                    break;
            }
        }
        else if(lineDelta < 0){
            int linesFromTop = currentLineNumber - (currentLineNumber - renderingData.currentLineViewOffset);
    
            for(int i = 0; i > lineDelta; i--){
                if(linesFromTop + i >= verticalScrollThreshold)
                    renderingData.currentLineViewOffset--;
    
                else if(currentLineNumber <= verticalScrollThreshold){
                    renderingData.currentLineViewOffset = currentLineNumber;
                } 
                else
                    break;
            }
        }
    
        renderingData.firstLineNumberShown = currentLineNumber - renderingData.currentLineViewOffset;
        renderingData.lastLineNumberShown = visableLines + renderingData.firstLineNumberShown; 
    }
    
    void ScrollViewHorizontal(const int& lineNumberColumnWidth, const int& termCol, const int& cursorCol, int& colOffset){
        while(cursorCol > termCol + colOffset - lineNumberColumnWidth){
            colOffset++;
        }
    
        while(cursorCol < colOffset){
            colOffset--; 
        }
    }

    void GetDataInView(const int& visableLines, const EditorRenderingState& renderingData, const IEditable& buffer,
            std::vector<std::string>& lines, std::vector<std::string>& lineNumbers){
        lines.reserve(visableLines);
        lineNumbers.reserve(visableLines);
    
        auto start = buffer.BeginStepsFromCurrentLine(-renderingData.currentLineViewOffset);
        auto end = buffer.EndStepsFromCurrentLine((visableLines-renderingData.currentLineViewOffset));
    
        for(auto line = start ; line != end; ++line){
            lines.push_back(*line);
            lineNumbers.push_back(std::to_string(line.LineNumber()));
        }
    
    }

    void CalculateLineNumberColumnValues(const std::vector<std::string>& lineNumbers, const int& minLineNumberColWidth, 
            int& lineNumberColumnWidth, int& lineNumberPaddingWidth){
        int lineNumberLength{};
        int maxLineNumberLength{};
    
        for(size_t i = 0; i < lineNumbers.size(); i++){
            lineNumberLength = lineNumbers[i].length();
            if(maxLineNumberLength < lineNumberLength)
                maxLineNumberLength = lineNumberLength;
        }
    
        lineNumberPaddingWidth = minLineNumberColWidth - maxLineNumberLength;
        if(lineNumberPaddingWidth < 0)
            lineNumberPaddingWidth *= -1;
        
        lineNumberColumnWidth = lineNumberPaddingWidth + maxLineNumberLength;
    }
};


TextEditor::TextEditor(Application& app, TerminalController& terminalController,
        ConfigVault& config) : pImpl(std::make_unique<Impl>(app, terminalController, config)){}

TextEditor::~TextEditor(){
    Stop();
}

void TextEditor::Start(std::string_view fileName, std::function<int()> getNextKey, 
        std::function<void(EditorRenderingState renderingState,
            std::vector<std::string>&& lines, std::vector<std::string>&& lineNumbers,
            int isCurrentLine)> renderEditor){
    pImpl->Start(fileName, getNextKey, renderEditor);
}

void TextEditor::Stop(){
    pImpl->Stop();
}
