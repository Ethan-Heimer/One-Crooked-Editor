#include "editor.h"
#include <memory>
#include <fstream>

Editor::Editor::Editor(const string fileName) : fileName(fileName), buffer(std::make_shared<Buffer>(fileName)){}

void Editor::Editor::Save(){
    ofstream saveFile{fileName};
    if(saveFile.is_open()){
        for(auto node : buffer->buffer){
            saveFile << ((string)*node).c_str() << endl;
        }

        saveFile.close();
    }
}
