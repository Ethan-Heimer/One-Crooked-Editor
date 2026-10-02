#pragma once

#include <cassert>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

//implement iterators
//delete from x to y
//insert range?
        
namespace GenericBuffer{
    enum class Options{
        Raw,
        Normalized
    };
     
    template<typename T>
    class GapBuffer{
        public:
            GapBuffer(unsigned int startingSize = 0, unsigned int expansionBredth = 5) {
                assert(expansionBredth > 0);
     
                this->expansionBredth = expansionBredth;
                if(startingSize > 0){
                    buffer.resize(startingSize);
                }
    
                startPtrIncl = 0;
                endPtrExcl = buffer.size();
            }

            ~GapBuffer(){
                buffer.clear();
            }

            GapBuffer(const GapBuffer& other){
                buffer = other.buffer;

                startPtrIncl = other.startPtrIncl;
                endPtrExcl = other.endPtrExcl;

                expansionBredth = other.expansionBredth;
            }

            GapBuffer(GapBuffer&& other){
                buffer = std::move(other.buffer);

                startPtrIncl = other.startPtrIncl;
                endPtrExcl = other.endPtrExcl;

                expansionBredth = other.expansionBredth;

                other.buffer = {};
                other.startPtrIncl = 0;
                other.endPtrExcl = 0;
            }

            GapBuffer& operator=(const GapBuffer& other){
                buffer = other.buffer;

                startPtrIncl = other.startPtrIncl;
                endPtrExcl = other.endPtrExcl;

                expansionBredth = other.expansionBredth;

                return *this;
            }

            GapBuffer& operator=(GapBuffer&& other){
                buffer = std::move(other.buffer);

                startPtrIncl = other.startPtrIncl;
                endPtrExcl = other.endPtrExcl;

                expansionBredth = other.expansionBredth;

                other.buffer = {};
                other.startPtrIncl = 0;
                other.endPtrExcl = 0;

                return *this;
            }
 
            void Insert(T element){
                if(startPtrIncl >= endPtrExcl){
                    ExpandGap();
                }
    
                buffer[startPtrIncl] = std::move(element);
                startPtrIncl++; 
            }

            void Remove(){
                if(startPtrIncl <= 0)
                    return;

                startPtrIncl = startPtrIncl - 1;
                buffer[startPtrIncl] = {};
            }
    
            void ExpandGap(){
                int oldSize = buffer.size();
                buffer.resize(oldSize + expansionBredth);
    
                for(int i = oldSize - 1; i >= endPtrExcl; i--)
                    std::swap(buffer[i], buffer[i + expansionBredth]);
    
                endPtrExcl += expansionBredth;
            }

            void MoveGapLeft(int steps){
                for(int i = 0; i < steps; i++)
                    MoveGapLeft();
            }

            void MoveGapLeft(){
                if(startPtrIncl <= 0)
                    return;

                startPtrIncl--;
                endPtrExcl--;

                std::swap(buffer[startPtrIncl], buffer[endPtrExcl]);
            }

            void MoveGapRight(int steps){
                for(int i = 0; i < steps; i++)
                    MoveGapRight();
            }

            void MoveGapRight(){
                if(endPtrExcl >= buffer.size())
                    return;

                std::swap(buffer[startPtrIncl], buffer[endPtrExcl]);

                endPtrExcl++;
                startPtrIncl++;
            }

            void MoveGapTo(int index){
                int difference = index - startPtrIncl;
                if(difference > 0)
                    MoveGapRight(difference);
                else if(difference < 0){
                    MoveGapLeft(-difference);
                }
            }
    
            size_t Size() const{
                return Size<Options::Normalized>();
            }
   
            template<Options Opt>
            constexpr size_t Size() const{
                if constexpr (Opt == Options::Raw)
                    return buffer.size();
                else 
                    return buffer.size() - (endPtrExcl - startPtrIncl); 
            }
    
    
            template<Options Opt>
            auto At(int index) const -> std::conditional_t<Opt == Options::Raw, const std::optional<T>&, const T&>{
                if constexpr (Opt == Options::Raw)
                    return buffer[index];
                else {
                    if(index < startPtrIncl)
                        return *buffer[index];
                    else
                        return *buffer[index + (endPtrExcl - startPtrIncl)];
                }
            }
    
    
            template<Options Opt>
            auto At(int index) -> std::conditional_t<Opt == Options::Raw, std::optional<T>&, T&>{
                if constexpr (Opt == Options::Raw)
                    return buffer[index];
                else {
                    if(index < startPtrIncl)
                        return *buffer[index];
                    else
                        return *buffer[index + (endPtrExcl - startPtrIncl)];
                }
            }

            const T& At(int index) const {
                return At<Options::Normalized>(index);
            }
    
            T& At(int index){
                return At<Options::Normalized>(index);
            }
            int GapStartRawIndex() const{
                return startPtrIncl;
            }
    
            int GapEndRawIndex() const{
                return endPtrExcl;
            }

            void* Data() const{
                return (void*)buffer.data();
            }

            const T& operator[](int index) const{
                return At(index);
            }

            T& operator[](int index){
                return At(index);
            }

            void operator++(){
                MoveGapRight();
            }

            void operator--(){
                MoveGapLeft();
            }

            void operator>>(int steps){
                MoveGapRight(steps);
            }

            void operator<<(int steps){
                MoveGapLeft(steps);
            }

            void operator >>= (int index){
                MoveGapTo(index);
            }

            void operator+=(T&& element){
                Insert(element);
            }

            friend bool operator==(GenericBuffer::GapBuffer<T>& a, 
                    GenericBuffer::GapBuffer<T>& b){
                return a.Data() == b.Data();
            }
            
        
        private:
            int expansionBredth;
    
            int startPtrIncl;
            int endPtrExcl; 
    
            std::vector<std::optional<T>> buffer;
    };

}
