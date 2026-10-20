#pragma once

#include "ringbuffer.hpp"
#include <atomic>

namespace Queues{
    template <typename T, int Size>
    class PingPongRingBuffer{
        public:
            PingPongRingBuffer(){
                writeBuffer.store(&bufferA, std::memory_order_release);
                readBuffer.store(&bufferB, std::memory_order_release);
            }
            void SwapBuffers(){
                auto writeAddress = writeBuffer.load(std::memory_order_acquire);
                auto readAddress = readBuffer.load(std::memory_order_acquire);
    
                readBuffer.store(writeAddress, std::memory_order_release);
                writeBuffer.store(readAddress, std::memory_order_release);
            }
    
            bool Push(const T& value){
                RingBuffer<T, Size>* buffer = writeBuffer.load(std::memory_order_acquire);
                return buffer->Push(value);
            }

            bool Produce(T&& value){
                RingBuffer<T, Size>* buffer = writeBuffer.load(std::memory_order_acquire);
                return buffer->Produce(std::forward<T>(value));
            }
    
            bool Pop(T& value){
                RingBuffer<T, Size>* buffer = readBuffer.load(std::memory_order_acquire);
                return buffer->Pop(value);
            }
    
            bool Consume(T& value){
                RingBuffer<T, Size>* buffer = readBuffer.load(std::memory_order_acquire);
                return buffer->Consume(value);
            }

            bool IsReadBufferEmpty(){
                RingBuffer<T, Size>* buffer = readBuffer.load(std::memory_order_acquire);
                return buffer->IsEmpty();
            }
            
            bool IsWriteBufferEmpty(){
                RingBuffer<T, Size>* buffer = writeBuffer.load(std::memory_order_acquire);
                return buffer->IsEmpty();
            }
        private:
            RingBuffer<T, Size> bufferA;
            RingBuffer<T, Size> bufferB;
    
            alignas(std::hardware_destructive_interference_size) std::atomic<RingBuffer<T, Size>*> writeBuffer;
            alignas(std::hardware_destructive_interference_size) std::atomic<RingBuffer<T, Size>*> readBuffer;
    };

}
