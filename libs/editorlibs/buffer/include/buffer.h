#pragma once

#include <sstream>

#include "filehandling.h"
#include "gapbuffer.h"

#include "doublyindexedlinkedlist.h"

using namespace FileHandling;

namespace Editor::Buffers{
    using Line = shared_ptr<DoublyIndexedLinkedList<GapBuffer>::Node>;

    class Buffer : public IFileLoadable, public IFileSaveable{
        public:
            DoublyIndexedLinkedList<GapBuffer> buffer;
            Line currentLine;

            Buffer();

            void ReadLineFromFile(const std::string& line) override;
            std::stringstream WriteLinesToFile() override;

            void GotoNextLine() noexcept;
            
            void GotoPreviousLine() noexcept;
            
            void MoveCursorLeft() noexcept;
            
            void MoveCursorRight() noexcept;
            
            bool IsCursorAtBeginningOfLine() noexcept;
            
            void InsertCharacter(char character) noexcept;
            void InsertString(string character) noexcept;
            
            void DeleteCharacter() noexcept;
            
            Line InsertLine() noexcept;
            
            void DeleteLine() noexcept;            

            void AppendTextToNextLine() noexcept;

            void MoveToHead() noexcept;
    };
}
