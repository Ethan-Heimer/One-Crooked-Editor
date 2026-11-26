#pragma once

#include <string>

#include "editorlineiterator.h"

namespace Editor{
    class IEditableEditCommands{            
        public:
            virtual void InsertCharacter(char character) noexcept = 0;
            virtual void InsertCharacterAt(unsigned int index, char character) noexcept = 0;
            virtual void InsertString(std::string_view character) noexcept = 0;
            virtual void InsertStringAt(unsigned int index, std::string_view string) noexcept = 0;
             
            virtual void InsertLine() noexcept = 0;
            
            virtual char DeleteCharacterAt(unsigned int index) noexcept = 0;
            virtual char DeleteCharacter() noexcept = 0;
            virtual void DeleteLine(std::string* remainingText) noexcept = 0;
            virtual void DeleteFromCol(unsigned int col, std::string* subString) noexcept = 0;

            virtual std::string SubstringBetween(unsigned int start, unsigned int end) noexcept = 0;

            virtual LineIterator Begin() const = 0;
            virtual LineIterator BeginAtCurrentLine() const = 0;
            virtual LineIterator BeginStepsFromCurrentLine(int steps) const = 0;

            virtual LineIterator End() const = 0;
            virtual LineIterator EndStepsFromCurrentLine(unsigned int steps) const = 0;
            virtual LineIterator AtLine(unsigned int line) const = 0 ;
    };
};
