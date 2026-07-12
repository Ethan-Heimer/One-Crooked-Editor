#pragma once

#include <array>
#include <cstdlib>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//Gap right is exclusive

class GapBuffer{
    public:
        GapBuffer(size_t startingSize, size_t resizeStride) : buffer(0, '\0'), resizeStride(resizeStride), gapStart(0), gapEnd(0){}
        GapBuffer(const string startingString, size_t resizeStride) : buffer(0, '\0'), resizeStride(resizeStride), gapStart(0), gapEnd(0){
            Insert(startingString);
        }

        void Grow(int width){
            vector<char> aBuf(buffer.size(), '\0');

            int j = 0;
            for(int i = gapEnd; i < buffer.size(); i++){
                aBuf[j] = buffer[i];
                j++; 
            }

            int newSize = buffer.size() + width;
            buffer.resize(newSize, '\0');
            gapEnd+=width;

            for(int i = gapStart ; i < gapEnd; i++){
                buffer[i] = '_';
            }
 
            j = 0;
            while(aBuf.size() > 0 && aBuf[j] != '\0'){
                buffer[gapEnd+j] = aBuf[j];
                j++;
            }
        }

        void MoveGapTo(unsigned int index){
            int difference = index - gapStart;
            MoveGapBy(difference);
        }

        void MoveGapBy(int steps){
            for(int i = 0; i < abs(steps); i++){
                if(steps < 0)
                    MoveGapLeft();
                else
                    MoveGapRight();
            }
        }

        void MoveGapLeft(){
            if(gapStart <= 0)
                return;

            gapStart--;
            gapEnd--;

            buffer[gapEnd] = buffer[gapStart];
            buffer[gapStart] = '_';

        }

        void MoveGapRight(){
            if(gapEnd >= buffer.size())
                return;

            buffer[gapStart] = buffer[gapEnd];
            buffer[gapEnd] = '_';

            gapStart++;
            gapEnd++;
        }

        void Insert(const char byte){
            if(buffer.size() == 0)
                Grow(resizeStride);

            buffer[gapStart] = byte;
            gapStart++;
            
            if(gapStart >= gapEnd-1){
                Grow(resizeStride);
            }
        }

        void Insert(const string string){
            for(int i = 0; i < string.length(); i++){
                char character = string[i];
                if(character == '\0')
                    continue;

                Insert(character);
            }
        }

        void InsertAt(unsigned int index, const char byte){
            MoveGapTo(index);
            Insert(byte);
        }

        void InsertAt(unsigned int index, const string string){
            MoveGapTo(index);
            Insert(string);
        }

        void Delete(){
            if(gapStart == 0)
                return;
            
            gapStart--;
            buffer[gapStart] = '_';
        }

        void DeleteBetween(unsigned int pointerOne, unsigned int pointerTwo){
            MoveGapTo(pointerTwo);
            while(GetGapIndex() != pointerOne){
                Delete();
            }
        }

        void DeleteAt(unsigned int index){
            MoveGapTo(index);
            Delete();
        }

        string RawContents(){
            stringstream stream;

            for(int i = 0; i < buffer.size(); i++){
                if(i == gapStart || i == gapEnd-1)
                    stream << '|';
                else
                    stream << buffer[i];
            }

            return stream.str();
        }

        string Substring(int from, int to) const{
            if(to < from)
                return string{};

            stringstream stream;
            int length = to - from;

            for(int i = from; length != 0 && i < buffer.size(); i++){
                if(i >= gapStart && i < gapEnd)
                    continue;
                
                stream << buffer[i];
                length--;
            }

            return stream.str(); 
        }

        string ToString() const{
            return Substring(0, buffer.size());
        }

        string BeforeGap(){
            stringstream stream;
            for(int i = 0; i < gapStart; i++){
                stream << buffer[i];
            }

            return stream.str();             
        }

        string AfterGap(){
            stringstream stream;
            for(int i = gapEnd; i < buffer.size(); i++){
                stream << buffer[i];
            }

            return stream.str();             
        }

        bool IsGapAtBeginning(){
            return gapStart == 0;
        }

        unsigned int EndIndex() const{
            return ToString().length();
        }

        unsigned int GetGapIndex() const{
            return gapStart;
        }

        unsigned int GetEndOfGap() const{
            return gapEnd;
        }

        unsigned int BufferSize() const {
            return buffer.size();
        }

        operator string() const{
            return ToString();
        }

        void operator>>(unsigned int steps){
            MoveGapBy(steps);
        }

        void operator<<(unsigned int steps){
            MoveGapBy(-steps);
        }

        void operator<<(const std::string input){
            Insert(input);
        }

        void operator<<(const char input){
            Insert(input);
        }

        GapBuffer& operator[](unsigned int index){
            MoveGapTo(index);
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& stream, const GapBuffer& buffer){
            stream << (string)buffer;
            return stream;
        }

    private:
        vector<char> buffer;
        unsigned int gapStart;
        unsigned int gapEnd;

        size_t resizeStride;
};
