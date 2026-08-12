#pragma once

#include "cursor.hpp"
#include "editorlineiterator.h"
#include "ieditable.h"
#include "rendercommandbase.hpp"
#include "terminal.hpp"
#include "tuitexture.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <format>

namespace CrookedEditor::Renderer {
    class RenderEditorCommand : public Rendering::Commands::RenderCommandBase{
        public:
            std::vector<std::string> lines;
            std::vector<std::string> lineNumbers;
            std::vector<bool> isCurrentLine;

            unsigned int cursorCol;
            unsigned int row, col;

            RenderEditorCommand(const Terminal::TerminalController& terminalController, const Editor::IEditable& buffer)
                : RenderCommandBase(terminalController){

                    terminalController.GetTerminalSize(row, col);
                    
                    lines.reserve(row);
                    lineNumbers.reserve(row);
                    isCurrentLine.reserve(row);

                    auto start = buffer.BeginStepsFromCurrentLine(-5);
                    auto end = buffer.EndStepsFromCurrentLine(row-1);
                    for(auto line = start ; line != end; ++line){
                        lines.push_back(*line);
                        lineNumbers.push_back(std::to_string(line.LineNumber()));
                        isCurrentLine.push_back(line.IsCurrentLine());
                    }

                    cursorCol = buffer.GetCursorX();
                }

            void Do(Rendering::CursorPosition& cursorPos, Rendering::TUITexture& frameBuffer) override{
                int lineColumnSpace = 6;

                int lineNumberLength{};
                int maxLineNumberLength{};

                std::string_view lineNumber{};
                std::string_view line{};

                for(size_t i = 0; i < lineNumbers.size(); i++) {
                    lineNumber = lineNumbers[i];

                    lineNumberLength = lineNumber.length();
                    for(int j = 0; j < lineNumberLength; j++){
                        frameBuffer.SetPixel(j, i, {lineNumber[j]});
                    }

                    if(maxLineNumberLength < lineNumberLength)
                        maxLineNumberLength = lineNumberLength;

                }

                int lineLength{};
                int lineNumberGapWidth = lineColumnSpace - maxLineNumberLength;
                if(lineNumberGapWidth < 0)
                    lineNumberGapWidth *= -1;
                   
                int lineNumberDisplayWidth = lineNumberGapWidth + maxLineNumberLength;
                int lineOffset = cursorCol - col + lineNumberDisplayWidth;
                if(lineOffset < 0)
                    lineOffset = 0;

                int termCursorLineNumber{};
                for(size_t i = 0; i < lines.size(); i++) { 
                    line = lines[i];
                    lineLength = line.length();

                    for(int j = 0; j < lineNumberGapWidth; j++){ 
                        frameBuffer.SetPixel(lineNumberLength + j, i, {' '});
                    }

                    for(int j = 0; j < lineLength - lineOffset; j++){
                        frameBuffer.SetPixel(lineNumberDisplayWidth+j, i, {line[lineOffset + j]});
                    }

                    if(isCurrentLine[i]){
                        termCursorLineNumber = i;
                    }
                }

                cursorPos.col = cursorCol + lineNumberDisplayWidth;
                cursorPos.row = termCursorLineNumber;

            }
    };
}
