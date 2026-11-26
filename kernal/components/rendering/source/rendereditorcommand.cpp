#include "rendereditorcommand.hpp"
#include "editorvisualstate.hpp"
#include "tuitexture.hpp"
#include <memory>
#include <string>

using namespace CrookedEditor::Renderer;
using namespace Rendering;
using namespace CrookedEditor::Components;


RenderEditorCommand::RenderEditorCommand(const Editor::IEditable& buffer, 
        Components::TextEditorVisualState& visualState, int row, int col){
    
    int currentLine = buffer.GetCurrentLineNumber();
    int currentCol = buffer.GetCursorX();

    // move document view
    while(currentLine > (row - 5 + visualState.rowOffset)){
        visualState.rowOffset++;
    }

    while(visualState.rowOffset > 0 && currentLine < 5 + visualState.rowOffset){ 
        visualState.rowOffset--;
    }

    while(currentCol > (col - 5 + visualState.colOffset)){
        visualState.colOffset ++;
    }

    while(visualState.colOffset > 0 && currentCol < visualState.colOffset){
        visualState.colOffset --;
    }

    //get line data
    auto begin = buffer.AtLine(visualState.rowOffset);
    auto end = buffer.AtLine(row + visualState.rowOffset);

    for(auto line = begin; line != end; ++line){
        if(line.CharacterCount() < visualState.colOffset)
            lines.push_back("");
        else
            lines.push_back((*line).substr(visualState.colOffset, col+visualState.colOffset));
    }

    //set rendering Data
    cursorRow = currentLine - visualState.rowOffset;
    cursorCol = currentCol - visualState.colOffset;
    rowOffset = visualState.rowOffset;
}

void RenderEditorCommand::Do(CursorPosition& cursorPos, TUITexture& frameBuffer){
    int displayOffset = 0;

    //render line numbers
    for(int i = 0; i < lines.size(); i++){
        std::string str = std::to_string(i+1+rowOffset);
        if(str.length() > displayOffset)
            displayOffset = str.length();

        for(int j = 0; j < str.length(); j++){
            frameBuffer.SetPixel(j, i, {str[j]});
        }
    }

    //render lines
    for(int row = 0; row < lines.size(); row++){
        for(int col = 0; col < lines[row].size(); col++){
            frameBuffer.SetPixel(col + displayOffset + 1, row, {lines[row][col]});
        }
    }

    cursorPos.row = cursorRow;
    cursorPos.col = cursorCol + displayOffset + 1;
}

std::unique_ptr<Rendering::Commands::IRenderCommand> RenderEditorCommand::Clone() const{
    return std::make_unique<RenderEditorCommand>(*this);
}
