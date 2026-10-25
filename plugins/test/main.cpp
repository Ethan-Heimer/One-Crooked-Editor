#include <fstream>

extern "C"{
    void start(){
        std::ofstream output{"testoutput.txt"};

        output << "Hello World 5000";

        output.close();
    }
}
