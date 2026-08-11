#pragma once

#include <cstring>
#include <iostream>
#include <vector>

namespace Rendering{
    struct Pixel{
        char character;
    };

    class TUITexture{
        public:
            TUITexture(){}
            TUITexture(unsigned int width, unsigned int height) : width(width), height(height){
                size_t channelSize = width * height;
                charChannel.reserve(channelSize);
                for(size_t i = 0; i < channelSize; i++)
                    charChannel.push_back(' ');
            }

            ~TUITexture(){}
            
            TUITexture(const TUITexture& other){
                width = other.width;
                height = other.height;

                charChannel = other.charChannel;
            }

            TUITexture(TUITexture&& other){
                width = other.width;
                height = other.height;

                charChannel = std::move(other.charChannel);

                other.width = 0;
                other.height = 0;
            }

            TUITexture& operator=(const TUITexture& other){
                width = other.width;
                height = other.height;

                charChannel = other.charChannel;

                return *this;
            }

            TUITexture& operator=(TUITexture&& other){
                width = other.width;
                height = other.height;

                charChannel = std::move(other.charChannel);
                
                other.width = 0;
                other.height = 0;

                return *this;
            }

            unsigned int Width() const{
                return width;
            }

            unsigned int Height() const{
                return height;
            }

            unsigned int Area() const{
                return width * height;
            }

            Pixel GetPixel(unsigned int x, unsigned int y){
                /* wrapping modes? */
                if(x > width)
                    x = width - 1;

                if(y > height)
                    y = height - 1;

                int index = y * width + x;
                return {charChannel[index]};
            }

            void SetPixel(unsigned int x, unsigned int y, Pixel pixel){
                if(x > width || y > height)
                    return;

                int index = y * width + x;
                charChannel[index] = pixel.character;
            }

            void Reinitialize(unsigned int width, unsigned int height){
                unsigned long capacity = width * height;
                if(Area() < capacity){
                    charChannel.reserve(capacity);
                } else if(Area() > capacity){
                    charChannel = std::vector<char>{};
                    charChannel.reserve(capacity);
                }

                for(unsigned long i = 0; i < charChannel.capacity(); i++)
                    charChannel.push_back(' ');

                this->width = width;
                this->height = height;
            }

            void Clear(){
                for(unsigned int i = 0; i < Area(); i++){
                    charChannel[i] = ' ';
                }
            }

            static const TUITexture& LargestTexture(const TUITexture& a, const TUITexture& b){
                return a.Area() > b.Area() ? a : b; 
            }

            static const TUITexture& SmallestTexture(const TUITexture& a, const TUITexture& b){
                return a.Area() < b.Area() ? a : b; 
            }

            static void DiffMap(const TUITexture& a, const TUITexture& b, std::vector<bool>& bitMap){
                const TUITexture& smallestTexture = SmallestTexture(a, b);
                unsigned int capacity = smallestTexture.Area();

                if(bitMap.capacity() < capacity){
                    bitMap.resize(capacity);
                }

                for(unsigned int i = 0; i < capacity; i++){
                    bitMap[i] = a.charChannel[i] != b.charChannel[i];
                }
            }

            static void CopyChannels(TUITexture& from, TUITexture& to){
                unsigned int capacity = from.Area();
                if(to.Area() != capacity)
                    to.Reinitialize(from.width, from.height);
                
                std::copy(from.charChannel.begin(), from.charChannel.end(), to.charChannel.begin());
            }

        private:
            unsigned int width{};
            unsigned int height{};

            std::vector<char> charChannel{};
            /* vectorized for future simd */
    };
}
