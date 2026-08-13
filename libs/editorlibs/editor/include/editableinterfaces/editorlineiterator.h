#pragma once

#include <memory>

namespace Editor {
    class ILineData{
        public:
            virtual std::shared_ptr<ILineData> NextLine() const = 0;
            virtual std::shared_ptr<ILineData> PreviousLine() const = 0;

            virtual std::string ToString() const = 0;
            virtual void* GetLineAddress() const = 0;
            virtual int LineNumber() const = 0;
            
            bool IsCurrentLine = false;
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
                if(a.currentLine == nullptr && b.currentLine==nullptr)
                    return true;

                if(a.currentLine == nullptr || b.currentLine == nullptr)
                    return false;

                return a.currentLine->GetLineAddress() == b.currentLine->GetLineAddress(); 
            };

            friend bool operator!=(const LineIterator& a, const LineIterator& b){
                if(a.currentLine == nullptr && b.currentLine==nullptr)
                    return false;

                if(a.currentLine == nullptr || b.currentLine == nullptr)
                    return true;

                return a.currentLine->GetLineAddress() != b.currentLine->GetLineAddress(); 
            };

            std::string operator*(){
               if(currentLine == nullptr) 
                   return "";

               return currentLine->ToString();
            };

            int LineNumber() const{
                if(currentLine == nullptr)
                    return 0;

                return currentLine->LineNumber();
            }

            bool IsCurrentLine() const{
                if(currentLine == nullptr)
                    return false;

                return currentLine->IsCurrentLine;
            }

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

            virtual LineIterator AtLine(unsigned int line) const = 0;
    };

}
