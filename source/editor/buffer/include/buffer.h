#pragma once

#include "gapbuffer/gapbuffer.h"
#include "linkedlist/doublyindexedlinkedlist.h"

namespace Editor::Buffers{
    using Line = shared_ptr<DoublyIndexedLinkedList<GapBuffer>::Node>;

    class Buffer{
        public:
            DoublyIndexedLinkedList<GapBuffer> buffer;
            Line currentLine;

            Buffer();

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
