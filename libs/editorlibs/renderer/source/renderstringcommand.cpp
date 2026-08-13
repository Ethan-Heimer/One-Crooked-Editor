#include "commands/standardrenderercommands.hpp"
#include "terminal.hpp"

using namespace Rendering::Commands;
using namespace Terminal;

RenderString::RenderString(const TerminalController& terminalController, int row, int col, std::string string)
    : RenderCommandBase(terminalController), string(string), row(row), col(col){}

void RenderString::Do(CursorPosition& cursorPos, TUITexture& frameBuffer){
    for(size_t i = 0; i < string.length(); i++)
        frameBuffer.SetPixel(col+i, row, {string[i]});
}
