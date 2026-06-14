#include <iostream>
#include "gapbuffer.h"

int main(){
    GapBuffer gapBuffer{"Testing", 5};
    std::cout << gapBuffer.RawContents() << std::endl;
    std::cout << gapBuffer.BufferSize() << std::endl;

    string subString = gapBuffer.Substring(0, 100);
    std::cout << subString << std::endl;
}
