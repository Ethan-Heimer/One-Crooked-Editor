#include "crookededitorcore.hpp"
#include <thread>

int main(int argc, char** argv){
    CrookedEditor::CrookedEditorCore crookedEditorCore{argc, argv};

    bool quit = false;
    crookedEditorCore.OnCoreQuit += [&](){
        quit = true;
    };

    while(!quit){
        std::this_thread::yield();
    }
}
