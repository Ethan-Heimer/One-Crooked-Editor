#include "buffer.h"
#include "fstream"

using namespace Editor::Buffers;

Buffer::Buffer(){
    buffer.Append("", 5);    
    currentLine = buffer.head;
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
            
Line Buffer::InsertLine() noexcept{
    return buffer.AppendAfter(currentLine, "", 5);
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
