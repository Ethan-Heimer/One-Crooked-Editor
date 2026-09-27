#include "buffer.h"
#include <memory>
#include <sstream>

using namespace CrookedEditor::Buffers;
using namespace Editor;

Buffer::Buffer(){
    buffer.Append(0, 10);    
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
    unsigned int currentLineNumber = GetCurrentLineNumber();

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
    int gapIndex = buffer.currentLine->data->GapStartRawIndex();
    return gapIndex == 0;
}
            
void Buffer::InsertCharacter(char character) noexcept{
    buffer.currentLine->data->Insert(character);
}

void Buffer::InsertString(string_view string) noexcept{
    for(int i = 0; i < string.length(); i++){
        buffer.currentLine->data->Insert(string[i]);
    }
}

void Buffer::InsertStringAt(unsigned int index, std::string_view string) noexcept{
    buffer.currentLine->data->MoveGapTo(index);
    InsertString(string);
}
            
char Buffer::DeleteCharacter() noexcept{
    char ch = '\0';
    int gapIndex = buffer.currentLine->data->GapStartRawIndex();
    if(gapIndex == 0)
        return ch;

    ch = buffer.currentLine->data->At(gapIndex-1);
    buffer.currentLine->data->Remove();
    return ch;
}
            
void Buffer::InsertLine() noexcept{
    buffer.AppendAfter(buffer.currentLine, 0, 10);
}
            
void Buffer::DeleteLine(std::string* remainingText) noexcept{
    bool hasPreviousLine = buffer.currentLine->previous.lock() != nullptr;
    if(!hasPreviousLine)
        return;

    int length = buffer.currentLine->data->Size();

    if(remainingText)
        *remainingText = SubstringBetween(0, length);

    buffer.Remove(buffer.currentLine);
    buffer.currentLine = buffer.currentLine->previous.lock();
}

void Buffer::DeleteFromCol(unsigned int col, std::string* subString) noexcept{
    int gapIndex = col;
    int endIndex = buffer.currentLine->data->Size();

    if(subString)
        *subString = SubstringBetween(gapIndex, endIndex);

    buffer.currentLine->data->MoveGapTo(endIndex);
    for(int i = gapIndex; i >= 0; i--){
        buffer.currentLine->data->Remove();
    }
}

std::string Buffer::SubstringBetween(unsigned int start, unsigned int end) noexcept{
    stringstream ss;
    for(int i = start; i < end; i++){
        ss << buffer.currentLine->data->At(i);
    }

    return ss.str();
}

void Buffer::MoveToHead() noexcept{
    buffer.currentLine = buffer.head;
}

unsigned int Buffer::GetCursorX() const noexcept{
    return buffer.currentLine->data->GapStartRawIndex();
}

unsigned int Buffer::GetCurrentLineNumber() const noexcept{
    return buffer.currentLine->index;
}

void Buffer::InsertCharacterAt(unsigned index, char character) noexcept{
    buffer.currentLine->data->MoveGapTo(index);
    buffer.currentLine->data->Insert(character);
}

char Buffer::DeleteCharacterAt(unsigned int index) noexcept{
    buffer.currentLine->data->MoveGapTo(index);
    return DeleteCharacter();
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

LineIterator Buffer::AtLine(unsigned int line) const{
    return buffer.AtLine(line);
}
