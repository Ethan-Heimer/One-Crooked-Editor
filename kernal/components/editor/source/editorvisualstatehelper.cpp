#include "editorvisualstatehelper.hpp"

using namespace CrookedEditor::Components;
using namespace Terminal;
using namespace Editor;

void EditorVisualStateHelper::UpdateVisualState(const TerminalController& terminalController, 
        const EditorContext& context, TextEditorVisualState& visualState, int invisableRows, 
        int verticalScrollThreshold, int lineColumnSpace){
    /* calculate visual offsets and send it off to the renderer */
    int row, col;
    terminalController.GetTerminalSize(row, col);

    visualState.cursorCol = context.buffer->GetCursorX();

    int currentLineNumber = context.buffer->GetCurrentLineNumber();
    int visableLines = row - invisableRows;

    ScrollViewVertical(verticalScrollThreshold, visableLines, currentLineNumber, visualState);
    ScrollViewHorizontal(6, col, visualState);

    int currentLine = context.buffer->GetCurrentLineNumber();

    GetDataInView(visableLines, visualState, *context.buffer);
    CalculateLineNumberColumnValues(visualState, lineColumnSpace);

    visualState.lastRowNumber = currentLineNumber;
}

void EditorVisualStateHelper::ScrollViewVertical(const int& verticalScrollThreshold, const int& visableLines, const int& currentLineNumber, TextEditorVisualState& visualState){
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
    
void EditorVisualStateHelper::ScrollViewHorizontal(const int& lineNumberColumnWidth, const int& termCol, TextEditorVisualState& visualState){
    while(visualState.cursorCol > termCol + visualState.colOffset - lineNumberColumnWidth){
        visualState.colOffset++;
    }
    
    while(visualState.cursorCol < visualState.colOffset){
        visualState.colOffset--; 
    }
}

void EditorVisualStateHelper::GetDataInView(const int& visableLines, TextEditorVisualState& visualState, const Editor::IEditable& buffer){
    visualState.lines.reserve(visableLines);
    visualState.lines.reserve(visableLines);
    
    auto start = buffer.BeginStepsFromCurrentLine(-visualState.currentLineViewOffset);
    auto end = buffer.EndStepsFromCurrentLine((visableLines-visualState.currentLineViewOffset));

    for(auto line = start; line != end; ++line){
        visualState.lines.push_back(*line);
        visualState.lineNumbers.push_back(std::to_string(line.LineNumber()));
    }
}

void EditorVisualStateHelper::CalculateLineNumberColumnValues(TextEditorVisualState& visualState, const int& minLineNumberColWidth){
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
