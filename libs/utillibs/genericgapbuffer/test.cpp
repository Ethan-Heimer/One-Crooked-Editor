#include <iostream>
#include <optional>
#include <print>
#include <sstream>
#include "genericgapbuffer.hpp"

using namespace GenericBuffer;

int main(){
    GapBuffer<char> buffer{0, 5};

    buffer.Insert('c');
    buffer.Insert('h');
    buffer.Insert('a');
    buffer.Insert('r');

    buffer.Insert(' ');
    buffer.Insert('s');
    buffer.Insert('t');
    buffer.Insert('a');
    buffer.Insert('r');

    //buffer << 3;
    buffer >>= 3;

    buffer.Insert('e');
    buffer.Insert('a');

    buffer >> 100;

    buffer.Insert('e');
    buffer += 'p';
    buffer += 'i';

    buffer << 3;

    buffer.Remove();
    buffer.Remove();

    buffer += '/';
    buffer += 't';

    std::stringstream ss;

    for(int i = 0; i < buffer.Size<GenericBuffer::Options::Raw>(); i++){
        std::optional<char>& element = buffer.At<GenericBuffer::Options::Raw>(i);
        if(element.has_value())
            ss << *element;
        else
            ss << '_';
    }

    std::cout << ss.str() << std::endl;

}
