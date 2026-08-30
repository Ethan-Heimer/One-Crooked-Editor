#pragma once

#include <strings.h>
#include <vector>

namespace Rendering{
    struct Pixel{
        char character;

        unsigned char fRed{255};
        unsigned char fGreen{255};
        unsigned char fBlue{255};
    };

    class TUITexture{
        public:
            TUITexture(){}
            TUITexture(int width, int height) : width(width), height(height){
                size_t channelSize = width * height;

                charChannel.reserve(channelSize);
                for(size_t i = 0; i < channelSize; i++){
                    charChannel.push_back(' ');
                    fRedChannel.push_back(255);
                    fBlueChannel.push_back(255);
                    fGreenChannel.push_back(255);
                }

            }

            ~TUITexture(){}
            
            TUITexture(const TUITexture& other){
                width = other.width;
                height = other.height;

                charChannel = other.charChannel;

                fRedChannel = other.fRedChannel;
                fBlueChannel = other.fBlueChannel;
                fGreenChannel = other.fGreenChannel;
            }

            TUITexture(TUITexture&& other){
                width = other.width;
                height = other.height;

                charChannel = std::move(other.charChannel);

                fRedChannel = std::move(other.fRedChannel);
                fBlueChannel = std::move(other.fBlueChannel);
                fGreenChannel = std::move(other.fGreenChannel);

                other.width = 0;
                other.height = 0;
            }

            TUITexture& operator=(const TUITexture& other){
                width = other.width;
                height = other.height;

                charChannel = other.charChannel;

                fRedChannel = other.fRedChannel;
                fBlueChannel = other.fBlueChannel;
                fGreenChannel = other.fGreenChannel;

                return *this;
            }

            TUITexture& operator=(TUITexture&& other){
                width = other.width;
                height = other.height;

                charChannel = std::move(other.charChannel);

                fRedChannel = std::move(other.fRedChannel);
                fBlueChannel = std::move(other.fBlueChannel);
                fGreenChannel = std::move(other.fGreenChannel);
                
                other.width = 0;
                other.height = 0;

                return *this;
            }

             int Width() const{
                return width;
            }

             int Height() const{
                return height;
            }

             int Area() const{
                return width * height;
            }

            Pixel GetPixel(int x, int y){
                /* wrapping modes? */
                if(x < 0)
                    x = 0;
                if(y < 0)
                    y = 0;

                if(x > width - 1)
                    x = width - 1;

                if(y > height - 1)
                    y = height - 1;

                 int index = y * width + x;
                return {charChannel[index], fRedChannel[index], fGreenChannel[index], fBlueChannel[index]};
            }

            void SetPixel( int x,  int y, Pixel pixel){
                if(x < 0 || y < 0)
                    return; 

                if(x > width-1 || y > height-1)
                    return;

                int index = y * width + x;

                charChannel[index] = pixel.character;

                fRedChannel[index] = pixel.fRed;
                fGreenChannel[index] = pixel.fGreen;
                fBlueChannel[index] = pixel.fBlue;
            }

            void Reinitialize(int width, int height){
                if(width < 0 || height < 0){
                    Clear();

                    this->width = width;
                    this->height = height;

                    return;
                }

                long capacity = width * height;
                if(Area() < capacity){
                    charChannel.reserve(capacity);

                    fRedChannel.reserve(capacity);
                    fGreenChannel.reserve(capacity);
                    fBlueChannel.reserve(capacity);

                } else if(Area() > capacity){
                    charChannel = std::vector<char>{};
                    charChannel.reserve(capacity);

                    fRedChannel = std::vector<unsigned char>{};
                    fRedChannel.reserve(capacity);

                    fGreenChannel = std::vector<unsigned char>{};
                    fGreenChannel.reserve(capacity);

                    fBlueChannel = std::vector<unsigned char>{};
                    fBlueChannel.reserve(capacity);
                }

                for(size_t i = 0; i < capacity; i++){
                    charChannel.push_back(' ');
                    
                    fRedChannel.push_back({});
                    fGreenChannel.push_back({});
                    fBlueChannel.push_back({});
                }

                this->width = width;
                this->height = height;
            }

            void Clear(){
                for(int i = 0; i < Area(); i++){
                    charChannel[i] = ' ';

                    fRedChannel[i] = 255;
                    fGreenChannel[i] = 255;
                    fBlueChannel[i] = 255;
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
                size_t capacity = smallestTexture.Area();

                if(bitMap.capacity() < capacity){
                    bitMap.resize(capacity);
                }

                for(size_t i = 0; i < capacity; i++){
                    bitMap[i] = a.charChannel[i] != b.charChannel[i] ||
                                a.fRedChannel[i] != b.fRedChannel[i] ||
                                a.fGreenChannel[i] != b.fGreenChannel[i] ||
                                a.fBlueChannel[i] != b.fBlueChannel[i];
                }
            }

            static void CopyChannels(TUITexture& from, TUITexture& to){
                int capacity = from.Area();
                if(to.Area() != capacity)
                    to.Reinitialize(from.width, from.height);
                
                std::copy(from.charChannel.begin(), from.charChannel.end(), to.charChannel.begin());

                std::copy(from.fRedChannel.begin(), from.fRedChannel.end(), to.fRedChannel.begin());
                std::copy(from.fGreenChannel.begin(), from.fGreenChannel.end(), to.fGreenChannel.begin());
                std::copy(from.fBlueChannel.begin(), from.fBlueChannel.end(), to.fBlueChannel.begin());
            }

        private:
            int width{};
            int height{};

            std::vector<char> charChannel{};

            std::vector<unsigned char> fRedChannel{};
            std::vector<unsigned char> fBlueChannel{};
            std::vector<unsigned char> fGreenChannel{};

            /* vectorized for future simd */
    };
}
