#include "requestbus.hpp"
#include <expected>
#include <iostream>
#include <string>
#include <unordered_map>


struct CopyTracer{
    int copies = 0;

    CopyTracer(){};
    ~CopyTracer(){
        std::cout << "Cop: " << copies << std::endl;
    }

    CopyTracer(const CopyTracer& other){
        std::cout << "COPYYYY" << std::endl;
        copies = other.copies + 1;
    }

    CopyTracer(CopyTracer&& other){
        copies = other.copies;
    }
};

struct AddConfig : public Events::IRequest<std::string, std::pair<std::string, std::string>>{};
struct GetConfig : public Events::IRequest<std::string, std::string>{};
struct CopyTest : public Events::IRequest<std::string, CopyTracer>{};
struct CopyTest2 : public Events::IRequest<CopyTracer, int>{};
struct VoidTest : public Events::IRequest<void, int>{};
struct Void2Test : public Events::IRequest<void, int>{};

int main(){
    Events::RequestBus bus;
    std::unordered_map<std::string, std::string> config;

    bus.AddRequest<AddConfig>([&](const std::pair<std::string, std::string>& pair){
        config[pair.first] = pair.second;
        return pair.second;
    });

    bus.AddRequest<GetConfig>([&](const std::string& key) -> GetConfig::request_return_type{
        if(!config.contains(key))
            return std::unexpected<std::string>("Key Not Found");
        else 
            return config[key];
    });

    bus.AddRequest<CopyTest>([&](const CopyTracer& tracer){
        return "Test";
    });

    bus.AddRequest<VoidTest>([&](int x){
        std::cout << x << std::endl;
    });

    bus.ListenForEvent<VoidTest>([](int x){
        std::cout << x + 20 << std::endl;
    });

    bus.AddRequest<CopyTest2>([&](const int& tracer){
        return CopyTracer{};
    });


    auto _ = bus.SendRequest<AddConfig>(std::pair<std::string, std::string>{"Test", "Hello World!"});
    auto res = bus.SendRequest<GetConfig>("Testoo");

    const auto& r = bus.SendRequest<CopyTest>(CopyTracer{});
    if(r.has_value())
        std::cout << r.value() << std::endl;
    
    bus.SendRequest<VoidTest>(10);
}
