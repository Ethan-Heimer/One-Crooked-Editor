#include "subprocess.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main(){
    /*
    Subprocess ls{"ls"};

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    char buffer[255] = {'\0'}; 
    size_t size = read(ls.output, &buffer[0], 255);

    std::cout << ls.processID << std::endl;
    std::cout << size << std::endl;
    */

    bool quit = false;
    std::thread job{[&](){
        Subprocess clangd{"ls", "-a"};

        while(clangd.IsAlive()){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            if(quit){
                clangd.Kill();
            }
            else
                std::cout << "Running..." << std::endl;
        }

        char buffer[255];
        read(clangd.output, buffer, 255);
        std::cout << buffer << std::endl;
    }};

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    quit = true;

    job.join();
}
