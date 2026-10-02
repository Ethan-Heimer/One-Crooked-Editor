#pragma once

#include "editorlineiterator.h"
#include "genericgapbuffer.hpp"

namespace CrookedEditor::Buffers {
    using LineType = GenericBuffer::GapBuffer<char>;
    using BufferType = GenericBuffer::GapBuffer<LineType>;

    class BufferData final : public Editor::ILineCollection{
        public:
            struct LineData final : Editor::ILineData{
                int CurrentIndex;
                const BufferData& Buffer;

                LineData(const BufferData& buffer, int index);

                std::shared_ptr<Editor::ILineData> NextLine() const override;
                std::shared_ptr<Editor::ILineData> PreviousLine() const override;

                std::string ToString() const override;
                int LineNumber() const override;
                void* GetAddress() const override;
            };


            const LineType& CurrentLine() const;
            LineType& CurrentLine();

            int GetCurentLineIndex() const;
            void SetCurrentLine(int index);

            size_t Size();
            void InsertLine();
            void DeleteLine();

            Editor::LineIterator Begin() const override;
            Editor::LineIterator BeginAtCurrentLine() const override;
            Editor::LineIterator BeginStepsFromCurrentLine(int steps) const override;

            Editor::LineIterator End() const override;
            Editor::LineIterator EndStepsFromCurrentLine(unsigned int steps) const override;
            Editor::LineIterator AtLine(unsigned int line) const override;

        private:
            BufferType data;
    };
}
