#pragma once

#include "editorlineiterator.h"
#include "gapbuffer.h"
#include "doublyindexedlinkedlist.h"

namespace CrookedEditor::Buffers {
    class BufferData final : public DoublyIndexedLinkedList<GapBuffer>, public Editor::ILineCollection{
        public:
            std::shared_ptr<Node> currentLine{head}; 

            struct LineData final : Editor::ILineData{
                std::shared_ptr<Node> line;
                LineData(std::shared_ptr<Node> line) : line(line){};

                std::shared_ptr<Editor::ILineData> NextLine() const override;
                std::shared_ptr<Editor::ILineData> PreviousLine() const override;

                std::string ToString() const override;
                void* GetLineAddress() const override;
                int LineNumber() const override;
            };

            Editor::LineIterator Begin() const override;
            Editor::LineIterator BeginAtCurrentLine() const override;
            Editor::LineIterator BeginStepsFromCurrentLine(int steps) const override;

            Editor::LineIterator End() const override;
            Editor::LineIterator EndStepsFromCurrentLine(unsigned int steps) const override;
    };
    using BufferNode = shared_ptr<BufferData::Node>;
}
