#pragma once

#include <mutex>
#include <queue>
#include <utility>
template<typename T>
class SafeQueue{
    public:
        SafeQueue(){};
        SafeQueue(const SafeQueue& other) = delete;
        SafeQueue(SafeQueue&& other){
            std::unique_lock<std::mutex> otherlock(other.mutex);

            std::swap(queue, other.queue);
        }
        ~SafeQueue() = default;

        SafeQueue& operator=(const SafeQueue& other) = delete;
        SafeQueue& operator=(SafeQueue&& other){
            std::unique_lock<std::mutex> otherlock(other.mutex);

            std::swap(queue, other.queue);

            return *this;
        }

        void push(T value){
            std::unique_lock<std::mutex> lock(mutex);
            queue.push(value);
        };

        void move(T&& value){
            std::unique_lock<std::mutex> lock(mutex);
            queue.push(std::move(value)); 
        }

        template<typename... U>
        void emplace(U&&... args){
            std::unique_lock<std::mutex> lock(mutex);
            queue.emplace(std::move(args...));  
        }

        bool empty() const{
            std::unique_lock<std::mutex> lock(mutex);
            return queue.empty();

        }

        size_t size() const{
            std::unique_lock<std::mutex> lock(mutex);
            return queue.size();
        }

        T& front() const{
            std::unique_lock<std::mutex> lock(mutex);
            return queue.front();
        }

        T& back() const{
            std::unique_lock<std::mutex> lock(mutex);
            return queue.back();
        };

        T& front(){
            std::unique_lock<std::mutex> lock(mutex);
            return queue.front();
        }

        T& back(){
            std::unique_lock<std::mutex> lock(mutex);
            return queue.back();
        };

        void pop(){
            std::unique_lock<std::mutex> lock(mutex);
            queue.pop();
        }
        
    private:
        mutable std::mutex mutex{};
        std::queue<T> queue{};
};
