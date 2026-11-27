#include "requestbus.hpp"
#include "requests.hpp"
#include <expected>
#include <iostream>
#include <string>
#include <tuple>
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
struct RemoveTest : public Events::IRequest<void, int, int>{};

struct RequestTest{
    Events::Request<RequestTest, void, int> test;

    void Send(){
        auto _ = test.Send(100);
    }
};

int main(){
    //reqest test
    RequestTest rt;
    auto func = [&](auto arg){
        std::cout << arg << std::endl;
    };

    rt.test.On(func);
    rt.Send();
    rt.test.RemoveCallback(func);
    rt.Send();

    // request bus test 
    Events::RequestBus bus;
    std::unordered_map<std::string, std::string> config;

    std::ignore = bus.AddRequest<AddConfig>([&](const std::pair<std::string, std::string>& pair){
        config[pair.first] = pair.second;
        return pair.second;
    });

    std::ignore = bus.AddRequest<GetConfig>([&](const std::string& key) -> GetConfig::request_return_type{
        if(!config.contains(key))
            return std::unexpected<std::string>("Key Not Found");
        else 
            return config[key];
    });

    std::ignore = bus.AddRequest<CopyTest>([&](const CopyTracer& tracer){
        return "Test";
    });

    std::ignore = bus.AddRequest<VoidTest>([&](int x){
        std::cout << x << std::endl;
    });

    std::ignore = bus.ListenForEvent<VoidTest>([](int x){
        std::cout << x + 20 << std::endl;
    });

    std::ignore = bus.AddRequest<CopyTest2>([&](const int& tracer){
        return CopyTracer{};
    });

    std::ignore = bus.AddRequest<RemoveTest>([&](int x, int y){
        std::cout << x << " " << y <<  std::endl;
    });

    auto x = [](int x, int y){
        std::cout << x << std::endl;
    };
    std::ignore = bus.ListenForEvent<RemoveTest>(x);

    auto y = [](int x, int y){
        std::cout << y << std::endl;
    };
    std::ignore = bus.ListenForEvent<RemoveTest>(y);


    auto _ = bus.SendRequest<AddConfig>(std::pair<std::string, std::string>{"Test", "Hello World!"});
    auto res = bus.SendRequest<GetConfig>("Testoo");

    const auto& r = bus.SendRequest<CopyTest>(CopyTracer{});
    if(r.has_value())
        std::cout << r.value() << std::endl;
    
    auto t = bus.SendRequest<VoidTest>(10);
    t = bus.SendRequest<RemoveTest>(50, 70);

    t = bus.RemoveEventListener<RemoveTest>(x);
    t = bus.SendRequest<RemoveTest>(50, 70);

    t = bus.RemoveRequest<RemoveTest>();
    t = bus.SendRequest<RemoveTest>(50, 70);
    if(!t.has_value())
        std::cout << t.error() << std::endl;
}
