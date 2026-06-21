#include "buffer.h"
#include "fstream"

using namespace Editor::Buffers;

Buffer::Buffer(string fileName){
    ifstream inputFile{fileName};

    string line;
    if(inputFile.is_open()){
        while(getline(inputFile, line)){
            buffer.Append(line, 5);
        }

        inputFile.close();
    }

    if(buffer.head)
        currentLine = buffer.head;
    else{
        buffer.Append("", 5);    
        currentLine = buffer.head;
    }
}

void Buffer::GotoNextLine(){
    if(currentLine->next)
        currentLine = currentLine->next;
}
            
void Buffer::GotoPreviousLine(){
    if(currentLine->previous.lock())
        currentLine = currentLine->previous.lock();
}
            
void Buffer::MoveCursorLeft(){
    currentLine->data->MoveGapLeft();
}
            
void Buffer::MoveCursorRight(){
    currentLine->data->MoveGapRight();
}
            
bool Buffer::IsCursorAtBeginningOfLine(){
    return currentLine->data->IsGapAtBeginning();
}
            
void Buffer::InsertCharacter(char character){
    currentLine->data->Insert(character);
}
            
void Buffer::DeleteCharacter(){
    currentLine->data->Delete();
}
            
Line Buffer::InsertLine(){
    return buffer.AppendAfter(currentLine, "", 5);
}
            
void Buffer::DeleteLine(){
    Line previousLine = currentLine->previous.lock();
    if(previousLine){
        string data = currentLine->data->ToString();
        previousLine->data->Insert(data);

        buffer.Remove(currentLine);
        if(currentLine->previous.lock())
            currentLine = currentLine->previous.lock();
    }
}
            
void Buffer::AppendTextToNextLine(){
    int gapIndex = currentLine->data->GetGapIndex();
    int endIndex = currentLine->data->BufferSize();
    string substring = currentLine->data->Substring(gapIndex, endIndex);
    currentLine->data->DeleteBetween(gapIndex, endIndex);
            
    if(currentLine->next){
        currentLine = currentLine->next;
        currentLine->data->Insert(substring);
        currentLine->data->MoveGapTo(0);
    }
}
