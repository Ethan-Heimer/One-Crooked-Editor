#pragma once

#include "ringbuffer.hpp"
#include <atomic>
#include <memory>

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

            size_t ReadBufferCount(){
                RingBuffer<T, Size>* buffer = readBuffer.load(std::memory_order_acquire);
                return buffer->Count();
            }

            size_t WriteBufferCount(){
                RingBuffer<T, Size>* buffer = writeBuffer.load(std::memory_order_acquire);
                return buffer->Count();
            }

            size_t WriteHeadIndex() const{
                RingBuffer<T, Size>* buffer = writeBuffer.load(std::memory_order_acquire);
                return buffer->HeadIndex();
            };

            size_t WriteTailIndex() const{
                RingBuffer<T, Size>* buffer = writeBuffer.load(std::memory_order_acquire);
                return buffer->TailIndex();
            };

            size_t ReadHeadIndex() const{
                RingBuffer<T, Size>* buffer = readBuffer.load(std::memory_order_acquire);
                return buffer->HeadIndex();
            };

            size_t ReadTailIndex() const{
                RingBuffer<T, Size>* buffer = readBuffer.load(std::memory_order_acquire);
                return buffer->TailIndex();
            };

            void const* ReadBufferAddress() const{
                RingBuffer<T, Size>* buffer = readBuffer.load(std::memory_order_acquire);
                return buffer;
            }

            void const* WriteBufferAddress() const{
                RingBuffer<T, Size>* buffer = writeBuffer.load(std::memory_order_acquire);
                return buffer;
            }


        private:
            RingBuffer<T, Size> bufferA;
            RingBuffer<T, Size> bufferB;
    
            alignas(std::hardware_destructive_interference_size) std::atomic<RingBuffer<T, Size>*> writeBuffer;
            alignas(std::hardware_destructive_interference_size) std::atomic<RingBuffer<T, Size>*> readBuffer;
    };

}
