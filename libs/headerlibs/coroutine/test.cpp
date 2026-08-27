#include <iostream>
#include <thread>
#include "coroutines.hpp"
#include <chrono>
#include <vector>

using namespace Coroutines;

std::vector<LazyAwaitable> lazyAwaitables{};

EagerCoroutine<std::string> StringTest(){
    lazyAwaitables.emplace_back();
    co_await lazyAwaitables[0];
    
    co_return "Done";
};

int main(){
    EagerCoroutine<std::string> coroutine = StringTest();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    lazyAwaitables[0].Resume();
    std::cout << coroutine.Value() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    return 0;
}
