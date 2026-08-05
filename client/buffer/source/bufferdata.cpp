#include "bufferdata.h"

using namespace CrookedEditor::Buffers;

std::shared_ptr<Editor::ILineData> BufferData::LineData::NextLine() const{
    if(!line)
        return nullptr;

    return std::make_shared<LineData>(line->next);
};

std::shared_ptr<Editor::ILineData> BufferData::LineData::PreviousLine() const{
    if(!line)
        return nullptr;

    return std::make_shared<LineData>(line->previous.lock());
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
    return line->index;
}

Editor::LineIterator BufferData::Begin() const {
    return Editor::LineIterator{make_shared<LineData>(head)};
};

Editor::LineIterator BufferData::BeginAtCurrentLine() const {
    return Editor::LineIterator{make_shared<LineData>(currentLine)};
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

    return Editor::LineIterator{make_shared<LineData>(currentNode)};
};

Editor::LineIterator BufferData::End() const{
    if(!tail)
        return Editor::LineIterator{make_shared<LineData>(nullptr)};

    return Editor::LineIterator{make_shared<LineData>(tail->next)};
}

Editor::LineIterator BufferData::EndStepsFromCurrentLine(unsigned int steps) const{
    std::shared_ptr<Node> currentNode = currentLine;
    int i = 0;
    while(currentNode->next && i < steps) {
        currentNode = currentNode->next; 
        i++;
    }

    return Editor::LineIterator{make_shared<LineData>(currentNode->next)};
};
