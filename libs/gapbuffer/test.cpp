#include <iostream>
#include <vector>
#include "gapbuffer.h"

int main(){
    std::cout << "Testing" << std::endl;

    GapBuffer buffer{10, 5};

    buffer.Grow(5);
    std::cout << buffer.RawContents() << std::endl;

    buffer.Grow(5);
    std::cout << buffer.RawContents() << std::endl;

    buffer.Insert("Hello");
    std::cout << buffer.RawContents() << std::endl;

    buffer.Insert("Wor");
    std::cout << buffer.RawContents() << std::endl;
    
    buffer.Insert("ld This is a Test!");
    std::cout << buffer.RawContents() << std::endl;

    buffer.Insert("A");
    std::cout << buffer.RawContents() << std::endl;

    buffer.Insert("B");
    std::cout << buffer.RawContents() << std::endl;

    buffer.Insert("C");
    std::cout << buffer.RawContents() << std::endl;

    buffer.Insert("D");
    std::cout << buffer.RawContents() << std::endl;

    buffer.Insert("E");
    std::cout << buffer.RawContents() << std::endl;

    buffer.MoveGapLeft();
    std::cout << buffer.RawContents() << std::endl;

    buffer.MoveGapLeft();
    std::cout << buffer.RawContents() << std::endl;

    buffer.MoveGapLeft();
    std::cout << buffer.RawContents() << std::endl;

    buffer.MoveGapRight();
    std::cout << buffer.RawContents() << std::endl;

    buffer.MoveGapRight();
    std::cout << buffer.RawContents() << std::endl;

    buffer.MoveGapRight();
    std::cout << buffer.RawContents() << std::endl;

    buffer.MoveGapRight();
    std::cout << buffer.RawContents() << std::endl;

    for(int i = 0; i < 3; i++){
        buffer.MoveGapLeft();
    }

    buffer.Insert("01234");
    std::cout << buffer.RawContents() << std::endl;

    std::cout << buffer << std::endl;

    buffer.MoveGapLeft();
    buffer.Insert("01234");
    std::cout << (string)buffer << std::endl;

    for(int i = 0; i < 15; i++){
        buffer.MoveGapLeft();
    }

    buffer.Insert("Slime");
    std::cout << buffer << std::endl;

    buffer >> 3;
    buffer << "Debug";

    buffer[4] << "Testing 1 2 3";
    std::cout << buffer << std::endl;
    std::cout << buffer.RawContents() << std::endl;

    std::cout << "Delete One by One" << std::endl;
    buffer.Delete();
    buffer.Delete();
    buffer.Delete();
    buffer.Delete();
    std::cout << buffer << std::endl;

    std::cout << "Testing Empty Buffer Outputs" << endl;

    GapBuffer buffer3{"", 5};
    int length = ((string)buffer3).length();
    std::cout << length << std::endl;
    std::cout << static_cast<int>(((string)buffer3)[0]) << std::endl;

    std::cout << "Testing Before After Gap" << endl;
    std::cout << buffer.RawContents() << endl;

    std::cout << buffer.BeforeGap() << endl;
    std::cout << buffer.AfterGap() << endl;

    buffer.MoveGapTo(100);
    buffer << "Hello";
    std::cout << buffer << std::endl;        

    std::cout << std::endl;
    std::cout << "Deleting Stuff" << std::endl;
    buffer << 'c';
    std::cout << buffer.RawContents() << std::endl;

    buffer.DeleteBetween(0, 10);
    std::cout << buffer.GetGapIndex() << endl;
    std::cout << buffer.GetEndOfGap() << endl;

    std::cout << buffer << std::endl;        

    unsigned int endIndex = buffer.EndIndex();
    buffer.DeleteBetween(12, endIndex);
    std::cout << buffer << std::endl;        

    string substring = buffer.GetSubstring(2, buffer.EndIndex());
    std::cout << substring << std::endl;

    buffer << "More Content Now Please!!!";
    std::cout << buffer << std::endl;        

    substring = buffer.GetSubstring(10, 24);
    buffer.DeleteBetween(10, 24);

    std::cout << buffer << endl;
    std::cout << substring << endl;
}
