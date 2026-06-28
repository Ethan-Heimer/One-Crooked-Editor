#pragma once

#include "editorfilehandling/ieditorfile.h"
#include <string>

using namespace std;
using namespace Editor::Files;

namespace Editor{
    class IEditable : public IEditorFile{
        public:
            virtual void GotoNextLine() noexcept = 0;
            
            virtual void GotoPreviousLine() noexcept = 0;
            
            virtual void MoveCursorLeft() noexcept = 0;
            
            virtual void MoveCursorRight() noexcept = 0;
            
            virtual bool IsCursorAtBeginningOfLine() noexcept = 0;
            
            virtual void InsertCharacter(char character) noexcept = 0;
            virtual void InsertString(string character) noexcept = 0;
            
            virtual void DeleteCharacter() noexcept = 0;
            
            virtual void InsertLine() noexcept = 0;
            
            virtual void DeleteLine() noexcept = 0;

            virtual void AppendTextToNextLine() noexcept = 0;

            virtual void MoveToHead() noexcept = 0;
    };
}
