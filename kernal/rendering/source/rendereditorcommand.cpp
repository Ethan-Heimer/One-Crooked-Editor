#include "rendereditorcommand.hpp"
#include "editorrenderingstate.hpp"
#include "tuitexture.hpp"
#include <vector>

using namespace CrookedEditor::Renderer;
using namespace Rendering;
using namespace Terminal;
using namespace Editor;

void RenderLineNumberColumn(const int& lineNumberPaddingWidth, const std::vector<std::string>& lineNumbers, TUITexture& frameBuffer);
void RenderLines(const std::vector<std::string>& lines, const int& startingLineNumber, const int& isCurrentLine, 
        const int& colOffset, const int& lineNumberColumnWidth, int& termCursorLineNumber, TUITexture& frameBuffer);

RenderEditorCommand::RenderEditorCommand(const Terminal::TerminalController& terminalController, EditorRenderingState renderingData,
        std::vector<std::string>&& lines, std::vector<std::string>&& lineNumbers, int currentLine)
    : RenderCommandBase(terminalController), renderingState(std::move(renderingData)),
      lines(lines), lineNumbers(lineNumbers), currentLine(currentLine){

    terminalController.GetTerminalSize(row, col);
}

void RenderEditorCommand::Do(CursorPosition& cursorPos, TUITexture& frameBuffer){
    int termCursorLineNumber{};
    int startingLineNumber = renderingState.firstLineNumberShown;

    RenderLineNumberColumn(renderingState.lineNumberPaddingWidth, lineNumbers, frameBuffer);
    RenderLines(lines, startingLineNumber, currentLine, renderingState.colOffset, 
            renderingState.lineNumberColumnWidth, termCursorLineNumber, frameBuffer);

    cursorPos.col = renderingState.cursorCol + renderingState.lineNumberColumnWidth - renderingState.colOffset;
    cursorPos.row = termCursorLineNumber;
}

void RenderLineNumberColumn(const int& lineNumberPaddingWidth, const std::vector<std::string>& lineNumbers, TUITexture& frameBuffer){
    std::string_view lineNumber{};
    int lineNumberLength{};
    for(size_t i = 0; i < lineNumbers.size(); i++) {
        lineNumber = lineNumbers[i];

        lineNumberLength = lineNumber.length();
        for(int j = 0; j < lineNumberLength; j++){
            frameBuffer.SetPixel(j, i, {lineNumber[j]});
        }

        for(int j = 0; j < lineNumberPaddingWidth; j++){ 
            frameBuffer.SetPixel(lineNumberLength + j, i, {' '});
        }
    }
}

void RenderLines(const std::vector<std::string>& lines, const int& startingLineNumber, const int& currentLine, 
        const int& colOffset, const int& lineNumberColumnWidth, int& termCursorLineNumber, TUITexture& frameBuffer){
    std::string_view line;
    int lineLength{};
    for(size_t i = 0; i < lines.size(); i++) { 
        line = lines[i];
        lineLength = line.length();


        // frame buffer is dealloceted while beig written to, and is therefore segfaulting?
        // this happens when the application is quit
        for(int j = 0; j < lineLength - colOffset; j++){
            frameBuffer.SetPixel(lineNumberColumnWidth+j, i, {line[colOffset + j]});
        }

        if(startingLineNumber + i == currentLine){
            termCursorLineNumber = i;
        }
    }

}
