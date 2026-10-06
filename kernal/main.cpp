#include "crookededitorcore.hpp"
#include <thread>

int main(int argc, char** argv){
    CrookedEditor::CrookedEditorCore crookedEditorCore{argc, argv};

    while(!crookedEditorCore.CoreQuit()){
        std::this_thread::yield();
    }
}
