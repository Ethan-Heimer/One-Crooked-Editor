#include "buffer.h"
#include <memory>

using namespace CrookedEditor::Buffers;
using namespace Editor;

Buffer::Buffer(){
    buffer.Append("", 5);    
    buffer.currentLine = buffer.head;
}

void Buffer::GotoNextLine() noexcept{
    if(buffer.currentLine->next)
        buffer.currentLine = buffer.currentLine->next;
}
            
void Buffer::GotoPreviousLine() noexcept{
    if(buffer.currentLine->previous.lock())
        buffer.currentLine = buffer.currentLine->previous.lock();
}

void Buffer::GotoLine(unsigned int lineNumber) noexcept{
    int currentLineNumber = GetCurrentLineNumber();

    if(bool traverseUp = currentLineNumber > lineNumber; traverseUp){
        while(buffer.currentLine->previous.lock() && buffer.currentLine->index != lineNumber){
            GotoPreviousLine();
        }
    }
    else if(bool traverseDown = currentLineNumber < lineNumber; traverseDown){
        while(buffer.currentLine->next && buffer.currentLine->index != lineNumber){
            GotoNextLine();
        }
    }
};
            
void Buffer::MoveCursorLeft() noexcept{
    buffer.currentLine->data->MoveGapLeft();
}
            
void Buffer::MoveCursorRight() noexcept{
    buffer.currentLine->data->MoveGapRight();
}

void Buffer::MoveCursorToCol(unsigned int col) noexcept{
    buffer.currentLine->data->MoveGapTo(col);
}
            
bool Buffer::IsCursorAtBeginningOfLine() const noexcept{
    return buffer.currentLine->data->IsGapAtBeginning();
}
            
void Buffer::InsertCharacter(char character) noexcept{
    buffer.currentLine->data->Insert(character);
}

void Buffer::InsertString(string_view string) noexcept{
    buffer.currentLine->data->Insert(string.data());
}

void Buffer::InsertStringAt(unsigned int index, std::string_view string) noexcept{
    buffer.currentLine->data->InsertAt(index, string.data());
}
            
char Buffer::DeleteCharacter() noexcept{
    return buffer.currentLine->data->Delete();
}
            
void Buffer::InsertLine() noexcept{
    buffer.AppendAfter(buffer.currentLine, "", 5);
}
            
void Buffer::DeleteLine() noexcept{
    auto previousLine = buffer.currentLine->previous.lock();
    if(previousLine){
        string data = buffer.currentLine->data->ToString();
        int endCursorPos = previousLine->data->EndIndex();

        previousLine->data->Insert(data);

        buffer.Remove(buffer.currentLine);
        buffer.currentLine = buffer.currentLine->previous.lock();
        buffer.currentLine->data->MoveGapTo(endCursorPos);
    }
}
            
void Buffer::AppendTextToNextLine() noexcept{
    if(buffer.currentLine->next == nullptr)
        return;

    int gapIndex = buffer.currentLine->data->GetGapIndex();
    int endIndex = buffer.currentLine->data->BufferSize();
    string substring = buffer.currentLine->data->Substring(gapIndex, endIndex);
    buffer.currentLine->data->DeleteBetween(gapIndex, endIndex);
            
    buffer.currentLine = buffer.currentLine->next;
    buffer.currentLine->data->Insert(substring);
    buffer.currentLine->data->MoveGapTo(0);
}

void Buffer::AppendTextToPreviousLine() noexcept{
    if(buffer.currentLine->previous.lock() == nullptr)
        return;

    int gapIndex = buffer.currentLine->data->GetGapIndex();
    int endIndex = buffer.currentLine->data->BufferSize();
    string substring = buffer.currentLine->data->Substring(gapIndex, endIndex);
    buffer.currentLine->data->DeleteBetween(gapIndex, endIndex);
            
    buffer.currentLine = buffer.currentLine->previous.lock();
    buffer.currentLine->data->Insert(substring);
}

std::string Buffer::SubstringBetween(unsigned int start, unsigned int end) noexcept{
    return buffer.currentLine->data->Substring(start, end);
}

void Buffer::MoveToHead() noexcept{
    buffer.currentLine = buffer.head;
}

unsigned int Buffer::GetCursorX() const noexcept{
    return buffer.currentLine->data->GetGapIndex();
}

unsigned int Buffer::GetCurrentLineNumber() const noexcept{
    return buffer.currentLine->index;
}

void Buffer::InsertCharacterAt(unsigned index, char character) noexcept{
    buffer.currentLine->data->InsertAt(index, character);
}

char Buffer::DeleteCharacterAt(unsigned int index) noexcept{
    return buffer.currentLine->data->DeleteAt(index);
}

LineIterator Buffer::Begin() const{
    return buffer.Begin();
}

LineIterator Buffer::BeginAtCurrentLine() const{
    return buffer.BeginAtCurrentLine();
}

LineIterator Buffer::BeginStepsFromCurrentLine(int steps) const{
    return buffer.BeginStepsFromCurrentLine(steps);
}

LineIterator Buffer::End() const{
    return buffer.End();
}

LineIterator Buffer::EndStepsFromCurrentLine(unsigned int steps) const{
    return buffer.EndStepsFromCurrentLine(steps);
}
