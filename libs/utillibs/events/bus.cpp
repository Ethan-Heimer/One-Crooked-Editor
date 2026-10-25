#include "eventbus.hpp"

#include <chrono>
#include <functional>
#include <thread>
#include <iostream>

struct TestTopic : public ITopic<int>{};
struct TestTopic2 : public ITopic<std::string>{};

int main(){
    EventBus bus;
    std::atomic<bool> quit;

    std::thread t1{[&](){
        bus.AddTopicAndListen<TestTopic>([&](int test){
            std::cout << "Test 1: " << test << std::endl;
        });

        while(!quit){
            bus.InvokeTopic<TestTopic2>("This is a test");
        }
    }};

    std::thread t2{[&](){
        bus.AddTopicAndListen<TestTopic2>([&](std::string x){
            std::cout << "Test 2: " << x << std::endl;
        });

        while(!quit){
            bus.InvokeTopic<TestTopic>(20);
        }
    }};

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    quit = true;

    t1.join();
    t2.join();
}
