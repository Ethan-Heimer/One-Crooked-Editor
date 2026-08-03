#pragma once

#include <memory>

namespace Editor {
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
            virtual LineIterator Begin() const = 0;
            virtual LineIterator BeginAtCurrentLine() const = 0;
            virtual LineIterator BeginStepsFromCurrentLine(int steps) const = 0;

            virtual LineIterator End() const = 0;
            virtual LineIterator EndStepsFromCurrentLine(unsigned int steps) const = 0;
    };

}
