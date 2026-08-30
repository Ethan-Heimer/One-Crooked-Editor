#include <iostream>

int Add(int a, int b);

int main(){
    std::cout << "hello world" << std::endl;

    std::cout << Add << std::endl;
    return 0;
}

int Add(int a, int b){
    return a + b;
}
