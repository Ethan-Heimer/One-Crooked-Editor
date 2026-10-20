#pragma once

#include <cstddef>
#include <atomic>
#include <new>
#include <utility>

namespace Queues{
    template<typename T, size_t Size>
    class RingBuffer{
        public:
            RingBuffer(){
                static_assert((Size & (Size - 1)) == 0, "Make sure ring buffer is a power of 2!");
            }
            bool Push(const T& value){
                //if the head is right behind the tail, the queue is full
                //(it cannot be  if head is at tail, because that could be either empty
                //or full)
                
                size_t headPtr = head.load(std::memory_order_acquire);
                size_t nextHeadPtr;
    
                do{
                    nextHeadPtr = (headPtr+1) & Size-1;
    
                    if(nextHeadPtr == tail.load(std::memory_order_acquire))
                        return false;
    
                }
                while(!head.compare_exchange_weak(headPtr, nextHeadPtr, std::memory_order_acq_rel));
    
                buffer[headPtr] = value;
                return true;
            }

            bool Produce(T&& value){
                //if the head is right behind the tail, the queue is full
                //(it cannot be  if head is at tail, because that could be either empty
                //or full)
                
                size_t headPtr = head.load(std::memory_order_acquire);
                size_t nextHeadPtr;
    
                do{
                    nextHeadPtr = (headPtr+1) & Size-1;
    
                    if(nextHeadPtr == tail.load(std::memory_order_acquire))
                        return false;
    
                }
                while(!head.compare_exchange_weak(headPtr, nextHeadPtr, std::memory_order_acq_rel));
    
                buffer[headPtr] = std::move(value);
                return true;
            }
    
            bool Pop(T& value){
                // if the tail is at the head, there is no data
                size_t tailPtr = tail.load(std::memory_order_acquire);
                do{
                    if(tailPtr == head.load(std::memory_order_acquire))
                        return false;
                }
                while(!tail.compare_exchange_weak(tailPtr, (tailPtr+1) & Size-1, std::memory_order_acq_rel));
    
                value = buffer[tailPtr];
                return true;
            }
    
            bool Consume(T& value){
                size_t tailPtr = tail.load(std::memory_order_acquire);
                do{
                    if(tailPtr == head.load(std::memory_order_acquire))
                        return false;
                }
                while(!tail.compare_exchange_weak(tailPtr, (tailPtr+1) & Size-1, std::memory_order_acq_rel));
    
                value = std::move(buffer[tailPtr]);
                return true;
            }
    
            size_t Capacity(){
                return Size;
            }
    
            bool IsEmpty(){
                return head.load(std::memory_order_acquire) == tail.load(std::memory_order_acquire);
            }
        private:
            constexpr static auto alignSize = 
                std::hardware_destructive_interference_size;
    
            alignas(alignSize) std::atomic<size_t> head = 0;
            alignas(alignSize) std::atomic<size_t> tail = 0;
    
            alignas(alignSize) std::array<T, Size> buffer;
    };

}
