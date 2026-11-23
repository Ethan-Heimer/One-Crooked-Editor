#include "ringbuffer.hpp"
#include <iostream>

int main(){
    Queues::RingBuffer<int, 64> buffer{};

    buffer.Push(10);
    buffer.Push(10);
    buffer.Push(10);
    buffer.Push(10);
    buffer.Push(10);
    buffer.Push(10);
    buffer.Push(10);

    std::cout << buffer.Count() << std::endl;
    std::cout << buffer.HeadIndex() << std::endl;
    std::cout << buffer.TailIndex() << std::endl;

    int value{};
    buffer.Pop(value);
    buffer.Pop(value);
    buffer.Pop(value);
    buffer.Pop(value);

    std::cout << buffer.Count() << std::endl;
    std::cout << buffer.HeadIndex() << std::endl;
    std::cout << buffer.TailIndex() << std::endl;
}
