#include "bufferdata.h"
#include "editorlineiterator.h"
#include "genericgapbuffer.hpp"
#include <cassert>
#include <memory>
#include <string>
#include <sstream>

using namespace CrookedEditor::Buffers;

BufferData::LineData::LineData(const BufferData& buffer, int index) 
    : Buffer(buffer), CurrentIndex(index){
        IsCurrentLine = Buffer.GetCurentLineIndex() == index;
    }

std::shared_ptr<Editor::ILineData> BufferData::LineData::NextLine() const{
    if(CurrentIndex + 1 >= Buffer.data.Size())
        return nullptr;

    return std::make_shared<LineData>(Buffer, CurrentIndex+1);
};

std::shared_ptr<Editor::ILineData> BufferData::LineData::PreviousLine() const{
    if(CurrentIndex - 1 < 0)
        return std::make_shared<LineData>(Buffer, 0);

    return std::make_shared<LineData>(Buffer, CurrentIndex-1);
};

std::string BufferData::LineData::ToString() const{
    assert(CurrentIndex < Buffer.data.Size());

    const LineType& line = Buffer.data.At(CurrentIndex);
    int end = line.Size();

    std::stringstream ss;

    for(int i = 0; i < end; i++){
        ss << Buffer.data.At(CurrentIndex).At(i);
    }

    return ss.str();
}

void* BufferData::LineData::GetAddress() const{
    assert(CurrentIndex < Buffer.data.Size());
    const LineType& line = Buffer.data.At(CurrentIndex);
    void* addr = (void*)&line;

    return addr;
}

int BufferData::LineData::LineNumber() const{
    return CurrentIndex + 1;
}

int BufferData::GetCurentLineIndex() const{
    // return the line before the gap
    int index = data.GapStartRawIndex()-1;
    return index;
}

void BufferData::SetCurrentLine(int index){
    // moves gap to after the line intended
    data.MoveGapTo(index+1);
}

size_t BufferData::Size(){
    return data.Size();
}

void BufferData::InsertLine(){
    data.Insert({0, 10});
}

void BufferData::DeleteLine(){
    //preserve first line
    if(data.Size() <= 1)
        return;

    data.Remove();
}

const LineType& BufferData::CurrentLine() const{
    return data.At(GetCurentLineIndex());
}

LineType& BufferData::CurrentLine(){
    int index = GetCurentLineIndex();
    return data.At(index);
}


Editor::LineIterator BufferData::Begin() const {
    if(data.Size() == 0)
        return Editor::LineIterator(nullptr);

    auto data = std::make_shared<LineData>(*this, 0);
    return Editor::LineIterator{data};
};

Editor::LineIterator BufferData::BeginAtCurrentLine() const {
    auto data = std::make_shared<LineData>(*this, GetCurentLineIndex());
    return Editor::LineIterator{data};
};

Editor::LineIterator BufferData::BeginStepsFromCurrentLine(int steps) const{
    int index = GetCurentLineIndex() + steps;
        
    if(index < 0)
        index = 0;

    if(index >= data.Size())
        return Editor::LineIterator(nullptr);

    auto data = std::make_shared<LineData>(*this, index);
    return Editor::LineIterator{data};    
};

Editor::LineIterator BufferData::End() const{
    return Editor::LineIterator{nullptr};
}

Editor::LineIterator BufferData::EndStepsFromCurrentLine(unsigned int steps) const{
    int index = GetCurentLineIndex() + steps;
    if(index >= data.Size())
        return Editor::LineIterator{nullptr};

    auto data = std::make_shared<LineData>(*this, index);
    return Editor::LineIterator{data};
};

Editor::LineIterator BufferData::AtLine(unsigned int line) const{
    auto data = std::make_shared<LineData>(*this, line);
    return Editor::LineIterator{data};
}
