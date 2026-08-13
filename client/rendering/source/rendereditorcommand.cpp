#include "rendereditorcommand.hpp"
#include "tuitexture.hpp"
#include <vector>

using namespace CrookedEditor::Renderer;
using namespace Rendering;
using namespace Terminal;
using namespace Editor;

void ScrollViewVertical(const int& verticalScrollThreshold, const int& visableLines, const int& currentLineNumber, EditorRenderingState& renderingData);
void ScrollViewHorizontal(const int& lineNumberColumnWidth, const int& termCol, const int& cursorCol, int& colOffset);

void CalculateLineNumberColumnValues(const std::vector<std::string>& lineNumbers ,const int& minLineNumberColWidth, int& lineNumberColumnWidth, int& lineNumberPaddingWidth);

void GetDataInView(const int& visableLines, const EditorRenderingState& renderingData, const IEditable& buffer,
        std::vector<std::string>& lines, std::vector<std::string>& lineNumbers, std::vector<bool>& isCurrentLine);

void CacheRowNumber(EditorRenderingState& renderingData, const int& currentLineNumber);

void RenderLineNumberColumn(const int& lineNumberPaddingWidth, const std::vector<std::string>& lineNumbers, TUITexture& frameBuffer);
void RenderLines(const std::vector<std::string>& lines, const std::vector<bool>& isCurrentLine, 
        const int& colOffset, const int& lineNumberColumnWidth, int& termCursorLineNumber, TUITexture& frameBuffer);

RenderEditorCommand::RenderEditorCommand(const TerminalController& terminalController, const IEditable& buffer, EditorRenderingState& renderingData)
    : RenderCommandBase(terminalController), colOffset(renderingData.colOffset){
    terminalController.GetTerminalSize(row, col);
    cursorCol = buffer.GetCursorX();

    int currentLineNumber = buffer.GetCurrentLineNumber();
    const int visableLines = row - invisableRows;
 
    ScrollViewVertical(verticalScrollThreshold, visableLines, currentLineNumber, renderingData);
    GetDataInView(visableLines, renderingData, buffer, lines, lineNumbers, isCurrentLine);

    CalculateLineNumberColumnValues(lineNumbers, lineColumnSpace, lineNumberColumnWidth, lineNumberPaddingWidth);

    ScrollViewHorizontal(lineNumberColumnWidth, col, cursorCol, colOffset);
    CacheRowNumber(renderingData, currentLineNumber);
}

void RenderEditorCommand::Do(CursorPosition& cursorPos, TUITexture& frameBuffer){
    int termCursorLineNumber{};

    RenderLineNumberColumn(lineNumberPaddingWidth, lineNumbers, frameBuffer);
    RenderLines(lines, isCurrentLine, colOffset, lineNumberColumnWidth, termCursorLineNumber, frameBuffer);

    cursorPos.col = cursorCol + lineNumberColumnWidth - colOffset;
    cursorPos.row = termCursorLineNumber;
}

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
        std::vector<std::string>& lines, std::vector<std::string>& lineNumbers, std::vector<bool>& isCurrentLine){
    lines.reserve(visableLines);
    lineNumbers.reserve(visableLines);
    isCurrentLine.reserve(visableLines);

    auto start = buffer.BeginStepsFromCurrentLine(-renderingData.currentLineViewOffset);
    auto end = buffer.EndStepsFromCurrentLine((visableLines-renderingData.currentLineViewOffset));

    for(auto line = start ; line != end; ++line){
        lines.push_back(*line);
        lineNumbers.push_back(std::to_string(line.LineNumber()));
        isCurrentLine.push_back(line.IsCurrentLine());
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

void CacheRowNumber(EditorRenderingState& renderingData, const int& currentLineNumber){
    renderingData.lastRowNumber = currentLineNumber;
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

void RenderLines(const std::vector<std::string>& lines, const std::vector<bool>& isCurrentLine, 
        const int& colOffset, const int& lineNumberColumnWidth, int& termCursorLineNumber, TUITexture& frameBuffer){
    std::string_view line;
    int lineLength{};
    for(size_t i = 0; i < lines.size(); i++) { 
        line = lines[i];
        lineLength = line.length();


        for(int j = 0; j < lineLength - colOffset; j++){
            frameBuffer.SetPixel(lineNumberColumnWidth+j, i, {line[colOffset + j]});
        }

        if(isCurrentLine[i]){
            termCursorLineNumber = i;
        }
    }

}
