#pragma once

namespace Editor{
    class IEditableCursorCommands{
        public:
            virtual void GotoNextLine() noexcept = 0;            
            virtual void GotoPreviousLine() noexcept = 0;
            virtual void GotoLine(unsigned int lineNumber) noexcept = 0;
            
            virtual void MoveCursorLeft() noexcept = 0;  
            virtual void MoveCursorRight() noexcept = 0;
            virtual void MoveCursorToCol(unsigned int col) noexcept = 0;

            virtual unsigned int GetCursorX() const noexcept = 0;
            virtual unsigned int GetCurrentLineNumber() const noexcept = 0;
            
            virtual bool IsCursorAtBeginningOfLine() const noexcept = 0;
            
            virtual void MoveToHead() noexcept = 0;

    };
}
