#include "standardrenderercommands.hpp"

using namespace Rendering::Commands;

ChangeCursorPosition::ChangeCursorPosition(int row, int col) : row(row), col(col){}

void ChangeCursorPosition::Do(CursorPosition& cursorPos, TUITexture& frameBuffer){
    cursorPos.row = row;
    cursorPos.col = col;
}
