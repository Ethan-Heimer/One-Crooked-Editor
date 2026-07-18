#include "buffer.h"
#include "ieditable.h"
#include <iostream>
#include <memory>
#include <sstream>

#define END_OF_BUFFER 0

using namespace Buffers;
using namespace Editor;

Buffer::Buffer(){
    buffer.Append("", 5);    
    buffer.currentLine = buffer.head;
}

void Buffer::ReadLineFromFile(const std::string& line){
    InsertString(line);
    InsertLine();
    GotoNextLine();
}

std::stringstream Buffer::WriteLinesToFile() {
    std::stringstream stream;
    for(auto line : buffer){
        stream << line.data << std::endl;
    }

    return stream;
}

void Buffer::GotoNextLine() noexcept{
    if(buffer.currentLine->next)
        buffer.currentLine = buffer.currentLine->next;
}
            
void Buffer::GotoPreviousLine() noexcept{
    if(buffer.currentLine->previous.lock())
        buffer.currentLine = buffer.currentLine->previous.lock();
}
            
void Buffer::MoveCursorLeft() noexcept{
    buffer.currentLine->data->MoveGapLeft();
}
            
void Buffer::MoveCursorRight() noexcept{
    buffer.currentLine->data->MoveGapRight();
}
            
bool Buffer::IsCursorAtBeginningOfLine() noexcept{
    return buffer.currentLine->data->IsGapAtBeginning();
}
            
void Buffer::InsertCharacter(char character) noexcept{
    buffer.currentLine->data->Insert(character);
}

void Buffer::InsertString(string_view string) noexcept{
    for(int i = 0; i < string.length(); i++){
        buffer.currentLine->data->Insert(string[i]);
    }
}
            
void Buffer::DeleteCharacter() noexcept{
    buffer.currentLine->data->Delete();
}
            
void Buffer::InsertLine() noexcept{
    buffer.AppendAfter(buffer.currentLine, "", 5);
}
            
void Buffer::DeleteLine() noexcept{
    auto previousLine = buffer.currentLine->previous.lock();
    if(previousLine){
        string data = buffer.currentLine->data->ToString();
        previousLine->data->Insert(data);

        buffer.Remove(buffer.currentLine);
        if(buffer.currentLine->previous.lock())
            buffer.currentLine = buffer.currentLine->previous.lock();
    }
}
            
void Buffer::AppendTextToNextLine() noexcept{
    int gapIndex = buffer.currentLine->data->GetGapIndex();
    int endIndex = buffer.currentLine->data->BufferSize();
    string substring = buffer.currentLine->data->Substring(gapIndex, endIndex);
    buffer.currentLine->data->DeleteBetween(gapIndex, endIndex);
            
    if(buffer.currentLine->next){
        buffer.currentLine = buffer.currentLine->next;
        buffer.currentLine->data->Insert(substring);
        buffer.currentLine->data->MoveGapTo(0);
    }
}

void Buffer::MoveToHead() noexcept{
    buffer.currentLine = buffer.head;
}

unsigned int Buffer::GetCursorX() noexcept{
    return buffer.currentLine->data->GetGapIndex();
}

unsigned int Buffer::GetCurrentLineNumber() noexcept{
    return buffer.currentLine->index;
}

void Buffer::InsertCharacterAt(unsigned index, char character) noexcept{
    buffer.currentLine->data->InsertAt(index, character);
}

void Buffer::DeleteCharacterAt(unsigned int index) noexcept{
    buffer.currentLine->data->DeleteAt(index);
}

LineIterator Buffer::Begin(){
    return buffer.Begin();
}

LineIterator Buffer::BeginAtCurrentLine(){
    return buffer.BeginAtCurrentLine();
}

LineIterator Buffer::BeginStepsFromCurrentLine(int steps){
    return buffer.BeginStepsFromCurrentLine(steps);
}

LineIterator Buffer::End(){
    return buffer.End();
}

LineIterator Buffer::EndStepsFromCurrentLine(unsigned int steps){
    return buffer.EndStepsFromCurrentLine(steps);
}
