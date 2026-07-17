#include <iostream>
#include "gapbuffer.h"

int main(){
    GapBuffer gapBuffer{"Testing", 5};
    std::cout << gapBuffer.RawContents() << std::endl;
    std::cout << gapBuffer.BufferSize() << std::endl;

    gapBuffer.MoveGapTo(3);
    std::cout << gapBuffer.RawContents() << std::endl;
    std::cout << gapBuffer.GetGapIndex() << std::endl;
}
