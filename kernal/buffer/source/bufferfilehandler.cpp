#include <memory>
#include <fstream>
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
        bool afterFirstLine = false;
        while(getline(inputFile, line)){
            if(afterFirstLine){
                buffer->InsertLine();
                buffer->GotoNextLine();
            }

            buffer->InsertString(line);
            if(!afterFirstLine)
                afterFirstLine=true;
        }

        inputFile.close();
    }

    buffer->MoveToHead();

    return std::move(buffer);
}

void BufferFileInterpreter::SaveToFile(std::string_view fileName, 
        const IEditable& buffer){
    ofstream saveFile{fileName.data()};
    if(saveFile.is_open()){
        auto start = buffer.Begin();
        auto end = buffer.End();

        for(auto line = start; line != end; ++line){
            saveFile << *line << endl;;
        }

        saveFile.close();
    }
}


