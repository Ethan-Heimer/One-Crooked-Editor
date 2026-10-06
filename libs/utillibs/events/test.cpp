#include <atomic>
#include <chrono>
#include <mutex>
#include <queue>
#include <thread>
#include "events.hpp"
#include <string>
#include <iostream>

class Timer{
    public:
        Event<Timer, std::string> OnTimerOff{};

        std::thread thread;

        std::mutex queueTex;
        std::queue<int> jobs;

        std::atomic<bool> quit;

        Timer(int sec){
            thread = std::thread{[&](){
                while(!quit){
                    std::this_thread::yield();

                    int seconds = 0;

                    queueTex.lock();
                    if(jobs.empty()){
                        queueTex.unlock();
                        continue;
                    }

                    seconds = jobs.front();
                    jobs.pop();
                    queueTex.unlock();

                    std::this_thread::sleep_for(std::chrono::seconds(seconds));
            
                    OnTimerOff.Invoke("Ding Ding Ding!");
                }
            }};

            queueTex.lock();
            jobs.push(sec);
            queueTex.unlock();
        };

        void Reset(int seconds){
            queueTex.lock();
            jobs.push(seconds);
            queueTex.unlock();
        }

        ~Timer(){
            quit = true;
            thread.join(); 
        }
};

class MemberTest{
    public:
        void Message(std::string message){
            std::cout << message << std::endl;
        };

        void MessageTwo(std::string message){
            std::cout << "I recieved: " << message << std::endl;
        }
};

int main(){
    bool timerOff{};

    Timer timer{10};
    MemberTest test{};
    
    timer.OnTimerOff += ([&](std::string message){
        std::cout << "Lambda: " << message << std::endl;
        timerOff = true;
    });

    timer.OnTimerOff += {test, &MemberTest::Message};
    timer.OnTimerOff.Subscribe(test, &MemberTest::MessageTwo);

    while(!timerOff){
        std::this_thread::yield();
    }

    timer.OnTimerOff.Unsubscribe(test, &MemberTest::Message);
    timerOff = false;
    timer.Reset(5);

    while(!timerOff){
        std::this_thread::yield();
    }

    timer.OnTimerOff.Subscribe(test, &MemberTest::Message);
    timer.OnTimerOff.Subscribe(test, &MemberTest::MessageTwo);

    timerOff = false;
    timer.Reset(5);

    while(!timerOff){
        std::this_thread::yield();
    }

    timer.OnTimerOff.UnsubscribeAll(test);

    std::cout << " " << std::endl;

    timerOff = false;
    timer.Reset(5);

    while(!timerOff){
        std::this_thread::yield();
    }
}
