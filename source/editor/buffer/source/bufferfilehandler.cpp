#include "bufferfilehandler.h"
#include "buffer.h"
#include "ibufferfilehandler.h"

#include <memory>
#include <fstream>

using namespace Editor;

Buffers::BufferFileHandler::BufferFileHandler(const string fileName)
    : IBufferFileHandler(fileName), fileName(fileName){}


std::shared_ptr<Buffers::Buffer> Buffers::BufferFileHandler::LoadNewBuffer() const{
    ifstream inputFile{fileName};

    std::shared_ptr<Buffers::Buffer> buffer = std::make_shared<Buffers::Buffer>();

    string line;
    if(inputFile.is_open()){
        while(getline(inputFile, line)){
            buffer->InsertString(line);
            buffer->InsertLine();
            buffer->GotoNextLine();
        }

        inputFile.close();
    }

    buffer->MoveToHead();

    return std::move(buffer);
}

void Buffers::BufferFileHandler::WriteBufferToFile(const std::shared_ptr<Buffers::Buffer>& buffer){
    ofstream saveFile{fileName};
    if(saveFile.is_open()){
        for(auto node : buffer->buffer){
            saveFile << ((string)*node).c_str() << endl;
        }

        saveFile.close();
    }
}
