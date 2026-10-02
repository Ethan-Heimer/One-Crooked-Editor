#include "buffer.h"
#include <fstream>
#include <sstream>

using namespace CrookedEditor::Buffers;
using namespace Editor;

Buffer::Buffer(){
    buffer.InsertLine();
    buffer.SetCurrentLine(0);
}

void Buffer::GotoNextLine() noexcept{
    if(buffer.GetCurentLineIndex() < buffer.Size()-1){
        int currentLineIndex = buffer.GetCurentLineIndex();
        buffer.SetCurrentLine(currentLineIndex + 1);
    }
}
            
void Buffer::GotoPreviousLine() noexcept{
    if(buffer.GetCurentLineIndex() > 0){
        int currentLineIndex = buffer.GetCurentLineIndex();
        buffer.SetCurrentLine(currentLineIndex - 1);
    }
}

void Buffer::GotoLine(unsigned int lineNumber) noexcept{
    if(lineNumber >= buffer.Size()){
        int index = buffer.Size() - 1;
        buffer.SetCurrentLine(index);
    }
    
    buffer.SetCurrentLine(lineNumber);
};
            
void Buffer::MoveCursorLeft() noexcept{
    buffer.CurrentLine().MoveGapLeft();
}
            
void Buffer::MoveCursorRight() noexcept{
    buffer.CurrentLine().MoveGapRight();
}

void Buffer::MoveCursorToCol(unsigned int col) noexcept{
    buffer.CurrentLine().MoveGapTo(col);
}
            
bool Buffer::IsCursorAtBeginningOfLine() const noexcept{
    int gapIndex = buffer.CurrentLine().GapStartRawIndex();
    return gapIndex == 0;
}
            

//the characters are not being put into the buffers
void Buffer::InsertCharacter(char character) noexcept{
    buffer.CurrentLine().Insert(character);
}

void Buffer::InsertString(std::string_view string) noexcept{
    for(int i = 0; i < string.length(); i++){
        LineType& line = buffer.CurrentLine();
        line.Insert(string[i]);
    }
}

void Buffer::InsertStringAt(unsigned int index, std::string_view string) noexcept{
    buffer.CurrentLine().MoveGapTo(index);
    InsertString(string);
}
            
char Buffer::DeleteCharacter() noexcept{
    char ch = '\0';
    int gapIndex = buffer.CurrentLine().GapStartRawIndex();
    if(gapIndex == 0)
        return ch;

    ch = buffer.CurrentLine().At(gapIndex-1);
    buffer.CurrentLine().Remove();
    return ch;
}
            
void Buffer::InsertLine() noexcept{
    buffer.InsertLine();
}
            
void Buffer::DeleteLine(std::string* remainingText) noexcept{
    int length = buffer.CurrentLine().Size();

    if(remainingText)
        *remainingText = SubstringBetween(0, length);
    
    buffer.DeleteLine();
}

void Buffer::DeleteFromCol(unsigned int col, std::string* subString) noexcept{
    int gapIndex = col;
    int endIndex = buffer.CurrentLine().Size();

    if(subString)
        *subString = SubstringBetween(gapIndex, endIndex);

    buffer.CurrentLine().MoveGapTo(endIndex);
    for(int i = endIndex; i > gapIndex; i--){
        buffer.CurrentLine().Remove();
    }
}

std::string Buffer::SubstringBetween(unsigned int start, unsigned int end) noexcept{
    std::stringstream ss;
    for(int i = start; i < end; i++){
        ss << buffer.CurrentLine().At(i);
    }

    return ss.str();
}

void Buffer::MoveToHead() noexcept{
    buffer.SetCurrentLine(0);
}

unsigned int Buffer::GetCursorX() const noexcept{
    return buffer.CurrentLine().GapStartRawIndex();
}

unsigned int Buffer::GetCurrentLineNumber() const noexcept{
    return buffer.GetCurentLineIndex();
}

void Buffer::InsertCharacterAt(unsigned index, char character) noexcept{
    buffer.CurrentLine().MoveGapTo(index);
    buffer.CurrentLine().Insert(character);
}

char Buffer::DeleteCharacterAt(unsigned int index) noexcept{
    buffer.CurrentLine().MoveGapTo(index);
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
