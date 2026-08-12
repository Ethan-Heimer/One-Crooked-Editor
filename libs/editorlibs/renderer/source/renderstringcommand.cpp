#include "standardrenderercommands.hpp"

using namespace Rendering::Commands;

RenderString::RenderString(int row, int col, std::string string) : string(string), row(row), col(col){}

void RenderString::Do(CursorPosition& cursorPos, TUITexture& frameBuffer){
    for(size_t i = 0; i < string.length(); i++)
        frameBuffer.SetPixel(col+i, row, {string[i]});
}
