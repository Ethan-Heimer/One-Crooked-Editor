#include "commands/standardrenderercommands.hpp"
#include "terminal.hpp"

using namespace Rendering::Commands;
using namespace Terminal;

ChangeCursorPosition::ChangeCursorPosition(const TerminalController& terminalController, unsigned int row, unsigned int col) 
    : RenderCommandBase(terminalController), row(row), col(col){}

void ChangeCursorPosition::Do(CursorPosition& cursorPos, TUITexture& frameBuffer){
    cursorPos.row = row;
    cursorPos.col = col;
}
