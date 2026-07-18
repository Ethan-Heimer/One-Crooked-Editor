#pragma once

#include "editorfilehandling/ieditorfile.h"
#include <memory>
#include <string_view>

namespace Editor{
    class Iterator;
    class ILineData{
        public:
        virtual std::shared_ptr<ILineData> NextLine() const = 0;
        virtual std::shared_ptr<ILineData> PreviousLine() const = 0;

        virtual std::string ToString() const = 0;
        virtual void* GetLineAddress() const = 0;
    };

    class LineIterator{
        public:
            LineIterator(std::shared_ptr<ILineData> currentLine)
                : currentLine(currentLine) {};
            
            LineIterator& operator++(){
                if(currentLine != nullptr)
                    currentLine = currentLine->NextLine(); 

                return *this;
            };

            friend bool operator==(const LineIterator& a, const LineIterator& b){
                return a.currentLine->GetLineAddress() == b.currentLine->GetLineAddress(); 
            };

            friend bool operator!=(const LineIterator& a, const LineIterator& b){
                return a.currentLine->GetLineAddress() != b.currentLine->GetLineAddress(); 
            };

            std::string operator*(){
               if(currentLine == nullptr) 
                   return "";

               return currentLine->ToString();
            };

        private:
            std::shared_ptr<ILineData> currentLine;
    };

    class ILineCollection{
        public:            
            virtual LineIterator Begin() = 0;
            virtual LineIterator BeginAtCurrentLine() = 0;
            virtual LineIterator BeginStepsFromCurrentLine(int steps) = 0;

            virtual LineIterator End() = 0;
            virtual LineIterator EndStepsFromCurrentLine(unsigned int steps) = 0;
    };

    class IEditable : public Files::IFile{
        public:
            virtual void GotoNextLine() noexcept = 0;            
            virtual void GotoPreviousLine() noexcept = 0;
            
            virtual void MoveCursorLeft() noexcept = 0;  
            virtual void MoveCursorRight() noexcept = 0;

            virtual unsigned int GetCursorX() noexcept = 0;
            virtual unsigned int GetCurrentLineNumber() noexcept = 0;
            
            virtual bool IsCursorAtBeginningOfLine() noexcept = 0;
            
            virtual void InsertCharacter(char character) noexcept = 0;
            virtual void InsertCharacterAt(unsigned int index, char character) noexcept = 0;
            virtual void InsertString(std::string_view character) noexcept = 0;
             
            virtual void InsertLine() noexcept = 0;
            
            virtual void DeleteCharacterAt(unsigned int index) noexcept = 0;
            virtual void DeleteCharacter() noexcept = 0;
            virtual void DeleteLine() noexcept = 0;

            virtual void AppendTextToNextLine() noexcept = 0;

            virtual LineIterator Begin() = 0;
            virtual LineIterator BeginAtCurrentLine() = 0;
            virtual LineIterator BeginStepsFromCurrentLine(int steps) = 0;

            virtual LineIterator End() = 0;
            virtual LineIterator EndStepsFromCurrentLine(unsigned int steps) = 0;

            virtual void MoveToHead() noexcept = 0;

            
    };
    
}
