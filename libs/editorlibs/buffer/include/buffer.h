#pragma once

#include <sstream>

#include "gapbuffer.h"

#include "doublyindexedlinkedlist.h"
#include "ieditable.h"

using namespace FileHandling;
using namespace Editor::Files;
using namespace Editor;

namespace Buffers{
    using Line = shared_ptr<DoublyIndexedLinkedList<GapBuffer>::Node>;

    class Buffer : public IEditable{
        public:
            DoublyIndexedLinkedList<GapBuffer> buffer;
            Line currentLine;

            Buffer();

            void ReadLineFromFile(const std::string& line) override;
            std::stringstream WriteLinesToFile() override;

            void GotoNextLine() noexcept override;
            
            void GotoPreviousLine() noexcept override;
            
            void MoveCursorLeft() noexcept override;
            
            void MoveCursorRight() noexcept override;
            
            bool IsCursorAtBeginningOfLine() noexcept override;
            
            void InsertCharacter(char character) noexcept override;
            void InsertCharacterAt(unsigned index, char character) noexcept override;

            void InsertString(string character) noexcept override;
            
            void InsertLine() noexcept override;

            void DeleteCharacterAt(unsigned int index) noexcept override;
            void DeleteCharacter() noexcept override; 
            
            void DeleteLine() noexcept override;

            void AppendTextToNextLine() noexcept override;

            void MoveToHead() noexcept override;
            unsigned int GetCursorX() noexcept override;
    };
}
