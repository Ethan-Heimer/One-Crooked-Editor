#include "buffer.h"
#include <sstream>

#define END_OF_BUFFER 0

using namespace Buffers;

Buffer::Buffer(){
    buffer.Append("", 5);    
    currentLine = buffer.head;
}

void Buffer::ReadLineFromFile(const std::string& line){
    InsertString(line);
    InsertLine();
    GotoNextLine();
}

std::stringstream Buffer::WriteLinesToFile() {
    std::stringstream stream;
    for(auto line : buffer){
        stream << *line << std::endl;
    }

    return stream;
}

void Buffer::GotoNextLine() noexcept{
    if(currentLine->next)
        currentLine = currentLine->next;
}
            
void Buffer::GotoPreviousLine() noexcept{
    if(currentLine->previous.lock())
        currentLine = currentLine->previous.lock();
}
            
void Buffer::MoveCursorLeft() noexcept{
    currentLine->data->MoveGapLeft();
}
            
void Buffer::MoveCursorRight() noexcept{
    currentLine->data->MoveGapRight();
}
            
bool Buffer::IsCursorAtBeginningOfLine() noexcept{
    return currentLine->data->IsGapAtBeginning();
}
            
void Buffer::InsertCharacter(char character) noexcept{
    currentLine->data->Insert(character);
}

void Buffer::InsertString(string string) noexcept{
    for(int i = 0; i < string.length(); i++){
        currentLine->data->Insert(string[i]);
    }
}
            
void Buffer::DeleteCharacter() noexcept{
    currentLine->data->Delete();
}
            
void Buffer::InsertLine() noexcept{
    buffer.AppendAfter(currentLine, "", 5);
}
            
void Buffer::DeleteLine() noexcept{
    Line previousLine = currentLine->previous.lock();
    if(previousLine){
        string data = currentLine->data->ToString();
        previousLine->data->Insert(data);

        buffer.Remove(currentLine);
        if(currentLine->previous.lock())
            currentLine = currentLine->previous.lock();
    }
}
            
void Buffer::AppendTextToNextLine() noexcept{
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

void Buffer::MoveToHead() noexcept{
    currentLine = buffer.head;
}
