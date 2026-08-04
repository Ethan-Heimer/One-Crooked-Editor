#pragma once

#include <sstream>

#include "ieditable.h"
#include "bufferdata.h"

namespace CrookedEditor::Buffers{
    class Buffer : public Editor::IEditable{
        public:
            BufferData buffer; 
            Buffer();

            void GotoNextLine() noexcept override; 
            void GotoPreviousLine() noexcept override;
            void GotoLine(unsigned int lineNumber) noexcept override;
            
            void MoveCursorLeft() noexcept override;            
            void MoveCursorRight() noexcept override;
            void MoveCursorToCol(unsigned int col) noexcept override;
            
            bool IsCursorAtBeginningOfLine() const noexcept override;
            
            void InsertCharacter(char character) noexcept override;
            void InsertCharacterAt(unsigned index, char character) noexcept override;

            void InsertString(string_view character) noexcept override;
            void InsertStringAt(unsigned int index, std::string_view string) noexcept override;
            
            void InsertLine() noexcept override;

            char DeleteCharacterAt(unsigned int index) noexcept override;
            char DeleteCharacter() noexcept override; 
            
            void DeleteLine() noexcept override;

            void AppendTextToNextLine() noexcept override;
            void AppendTextToPreviousLine() noexcept override;

            std::string SubstringBetween(unsigned int start, unsigned int end) noexcept override;

            void MoveToHead() noexcept override;

            virtual Editor::LineIterator Begin() const override;
            virtual Editor::LineIterator BeginAtCurrentLine() const override;
            virtual Editor::LineIterator BeginStepsFromCurrentLine(int steps) const override;

            virtual Editor::LineIterator End() const override;
            virtual Editor::LineIterator EndStepsFromCurrentLine(unsigned int steps) const override;

            unsigned int GetCursorX() const noexcept override;
            unsigned int GetCurrentLineNumber() const noexcept override;
    };
}
