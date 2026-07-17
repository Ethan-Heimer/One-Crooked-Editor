#include <memory>
#include <fstream>
#include <sstream>
#include <string>

#include "bufferfilehandler.h"
#include "buffer.h"
#include "ibufferfilehandler.h"

using namespace Editor;

Buffers::BufferFileHandler::BufferFileHandler(const string fileName)
    : IBufferFileHandler(fileName), fileName(fileName){}


std::shared_ptr<Buffers::Buffer> Buffers::BufferFileHandler::LoadFromFile(){
    ifstream inputFile{fileName};
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

void Buffers::BufferFileHandler::SaveToFile(const std::shared_ptr<IFileSaveable> fileSaver){
    ofstream saveFile{fileName};
    if(saveFile.is_open()){
        stringstream stream = fileSaver->WriteLinesToFile();
        string line;
        while(std::getline(stream, line)){
            saveFile << line << endl;;
        }

        saveFile.close();
    }

}


