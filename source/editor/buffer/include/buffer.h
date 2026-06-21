#pragma once

#include "gapbuffer/gapbuffer.h"
#include "linkedlist/doublyindexedlinkedlist.h"

namespace Editor::Buffers{
    using Line = shared_ptr<DoublyIndexedLinkedList<GapBuffer>::Node>;

    class Buffer{
        public:
            DoublyIndexedLinkedList<GapBuffer> buffer;
            Line currentLine;

            Buffer(string fileName);

            void GotoNextLine();
            
            void GotoPreviousLine();
            
            void MoveCursorLeft();
            
            void MoveCursorRight();
            
            bool IsCursorAtBeginningOfLine();
            
            void InsertCharacter(char character);
            
            void DeleteCharacter();
            
            Line InsertLine();
            
            void DeleteLine();
            
            void AppendTextToNextLine();
    };
}
