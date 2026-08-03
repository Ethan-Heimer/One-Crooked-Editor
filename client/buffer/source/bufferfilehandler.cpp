#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include "bufferfilehandler.h"
#include "buffer.h"

using namespace CrookedEditor::Buffers;
using namespace Editor;
using namespace std;

std::shared_ptr<IEditable> BufferFileInterpreter::LoadFromFile(std::string_view fileName){
    ifstream inputFile{fileName.data()};
    std::shared_ptr<Buffers::Buffer> buffer = std::make_shared<Buffers::Buffer>();

    string line;
    if(inputFile.is_open()){
        while(getline(inputFile, line)){
            buffer->ReadLineFromFile(line);
        }

        inputFile.close();
    }

    buffer->MoveToHead();

    return std::move(buffer);
}

void BufferFileInterpreter::SaveToFile(std::string_view fileName, 
        const std::shared_ptr<const IEditableFileCommands>& fileSaver){

    ofstream saveFile{fileName.data()};
    if(saveFile.is_open()){
        stringstream stream = fileSaver->WriteLinesToFile();
        string line;
        while(std::getline(stream, line)){
            saveFile << line << endl;;
        }

        saveFile.close();
    }
}


