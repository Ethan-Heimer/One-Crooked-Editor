#pragma once

#include <cstddef>
#include <atomic>
#include <cstdint>
#include <iostream>
#include <new>
#include <thread>
#include <utility>
#include <array>

namespace Queues{
    template<typename T, size_t Size>
    class RingBuffer{
        public:
            struct Slot{
                T data;
                std::atomic<int64_t> sequence;     
            };

            RingBuffer(){
                static_assert((Size & (Size - 1)) == 0, "Make sure ring buffer is a power of 2!");

                for(int i = 0; i < Size; i++){
                    buffer[i].sequence.store(i, std::memory_order_release);
                }
            }

            bool Push(const T& value){
                int64_t currentEnqueueSeq = 0; 
                size_t slotIndex = 0;

                while(true){
                    currentEnqueueSeq = enqueueSeq.load(std::memory_order_acquire);
                    slotIndex = currentEnqueueSeq & Size-1;


                    Slot& slot = buffer[slotIndex];
                    int64_t slotSeq = slot.sequence.load(std::memory_order_acquire);
                    int64_t diff = slotSeq - currentEnqueueSeq;

                    if(diff == 0){
                        if(enqueueSeq.compare_exchange_weak(currentEnqueueSeq, currentEnqueueSeq + 1, std::memory_order_acq_rel)){
                            break;
                        }
                    }
                    else if(diff < 0){
                        return false;
                    } else {
                        std::this_thread::yield();
                        continue;
                    }
                }

                Slot& slot = buffer[slotIndex];
                slot.data = value;
                slot.sequence.store(currentEnqueueSeq + 1, std::memory_order_release);

                return true;
            }

    
            bool Pop(T& value){
                int64_t currentDequeueSeq = 0; 
                size_t slotIndex = 0;

                while(true){
                    currentDequeueSeq = dequeueSeq.load(std::memory_order_acquire);
                    slotIndex = currentDequeueSeq & Size-1;

                    Slot& slot = buffer[slotIndex];
                    int64_t slotSeq = slot.sequence.load(std::memory_order_acquire);
                    int64_t diff = slotSeq - (currentDequeueSeq + 1);

                    if(diff == 0){
                        if(dequeueSeq.compare_exchange_weak(currentDequeueSeq, currentDequeueSeq + 1, std::memory_order_acq_rel))
                            break;
                    } else if(diff < 0){
                            return false;
                    } else {
                        std::this_thread::yield();
                        continue;
                    }
                }

                Slot& slot = buffer[slotIndex];
                value = slot.data;
                slot.sequence.store(currentDequeueSeq + Size, std::memory_order_release);

                return true;
            }

            bool Produce(T&& value){
                int64_t currentEnqueueSeq = 0; 
                size_t slotIndex = 0;
                while(true){
                    currentEnqueueSeq = enqueueSeq.load(std::memory_order_acquire);
                    slotIndex = currentEnqueueSeq & Size-1;

                    Slot& slot = buffer[slotIndex];
                    int64_t slotSeq = slot.sequence.load(std::memory_order_acquire);
                    int64_t diff = slotSeq - currentEnqueueSeq;

                    if(diff == 0){
                        if(enqueueSeq.compare_exchange_weak(currentEnqueueSeq, currentEnqueueSeq + 1, std::memory_order_acq_rel)){
                            break;
                        }
                    }
                    else if(diff < 0){
                        return false;
                    } else {
                        std::this_thread::yield();
                        continue;
                    }
                }

                Slot& slot = buffer[slotIndex];
                slot.data = std::forward<T>(value);
                slot.sequence.store(currentEnqueueSeq + 1, std::memory_order_release);

                return true;
            }
    
            bool Consume(T& value){
                int64_t currentDequeueSeq = 0; 
                size_t slotIndex = 0;

                while(true){
                    currentDequeueSeq = dequeueSeq.load(std::memory_order_acquire);
                    slotIndex = currentDequeueSeq & Size-1;

                    Slot& slot = buffer[slotIndex];
                    int64_t slotSeq = slot.sequence.load(std::memory_order_acquire);
                    int64_t diff = slotSeq - (currentDequeueSeq + 1);

                    if(diff == 0){
                        if(dequeueSeq.compare_exchange_weak(currentDequeueSeq, currentDequeueSeq + 1, std::memory_order_acq_rel))
                            break;
                    } else if(diff < 0){
                            return false;
                    } else {
                        std::this_thread::yield();
                        continue;
                    }
                }

                Slot& slot = buffer[slotIndex];
                value = std::move(slot.data);
                slot.sequence.store(currentDequeueSeq + Size, std::memory_order_release);

                return true;
            }
    
            size_t Capacity(){
                return Size;
            }

            size_t Count(){
                size_t headPtr = enqueueSeq.load(std::memory_order_acquire);
                size_t tailPtr = dequeueSeq.load(std::memory_order_acquire);

                return headPtr - tailPtr;
                 
            }

            size_t HeadIndex() const{
                return enqueueSeq.load(std::memory_order_acquire);
            }

            size_t TailIndex() const{
                return dequeueSeq.load(std::memory_order_acquire);
            }
        private:
            constexpr static auto alignSize = 
                std::hardware_destructive_interference_size;
    
            alignas(alignSize) std::atomic<int64_t> enqueueSeq = 0;
            alignas(alignSize) std::atomic<int64_t> dequeueSeq = 0;
    
            alignas(alignSize) std::array<Slot, Size> buffer;
    };

}
