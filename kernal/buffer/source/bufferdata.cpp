#include "bufferdata.h"

using namespace CrookedEditor::Buffers;

BufferData::LineData::LineData(std::shared_ptr<Node> line, std::weak_ptr<const Node> currentLine) 
    : line(line), currentLine(currentLine){
        if(!line || !currentLine.lock())
            IsCurrentLine = false;

        IsCurrentLine = line == currentLine.lock();
}

std::shared_ptr<Editor::ILineData> BufferData::LineData::NextLine() const{
    if(!line)
        return nullptr;

    return std::make_shared<LineData>(line->next, currentLine);
};

std::shared_ptr<Editor::ILineData> BufferData::LineData::PreviousLine() const{
    if(!line)
        return nullptr;

    return std::make_shared<LineData>(line->previous.lock(), currentLine);
};

std::string BufferData::LineData::ToString() const{
    if(!line)
        return "";

    return line->data->ToString();
}

void* BufferData::LineData::GetLineAddress() const{
    if(!line)
        return nullptr;

    return line.get();
}

int BufferData::LineData::LineNumber() const{
    if(!line)
        return 0;

    return line->index;
}


Editor::LineIterator BufferData::Begin() const {
    return Editor::LineIterator{make_shared<LineData>(head, currentLine)};
};

Editor::LineIterator BufferData::BeginAtCurrentLine() const {
    return Editor::LineIterator{make_shared<LineData>(currentLine, currentLine)};
};

Editor::LineIterator BufferData::BeginStepsFromCurrentLine(int steps) const{
    std::shared_ptr<Node> currentNode = currentLine;

    int i = 0;
    if(steps < 0){
        while(currentNode->previous.lock() && i < steps * -1) {
            currentNode = currentNode->previous.lock(); 
            i++;
        }
    } else {
        while(currentNode->next && i < steps) {
            currentNode = currentNode->next; 
            i++;
        }
    }

    return Editor::LineIterator{make_shared<LineData>(currentNode, currentLine)};
};

Editor::LineIterator BufferData::End() const{
    if(!tail)
        return Editor::LineIterator{make_shared<LineData>(nullptr, currentLine)};

    return Editor::LineIterator{make_shared<LineData>(tail->next, currentLine)};
}

Editor::LineIterator BufferData::EndStepsFromCurrentLine(unsigned int steps) const{
    std::shared_ptr<Node> currentNode = currentLine;
    unsigned int i = 0;
    while(currentNode->next && i < steps) {
        currentNode = currentNode->next; 
        i++;
    }

    return Editor::LineIterator{make_shared<LineData>(currentNode->next, currentLine)};
};

Editor::LineIterator BufferData::AtLine(unsigned int line) const{
    std::shared_ptr<Node> currentNode = head;
    unsigned int i = 0;
    while(currentNode && i < line) {
        currentNode = currentNode->next; 
        i++;
    }
     
    return Editor::LineIterator{make_shared<LineData>(currentNode, currentLine)};
}
