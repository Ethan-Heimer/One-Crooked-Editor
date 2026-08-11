#pragma once

#include "rendercommand.hpp"
#include "tuitexture.hpp"

namespace Rendering::Commands{
    class RenderString : public Rendering::Commands::RenderCommand{
        public:
            std::string string;
            int row{}, col{};

            RenderString(int col, int row, std::string string) : string(string), row(row), col(col){}
            void Do(TUITexture& frameBuffer) override{
                for(size_t i = 0; i < string.length(); i++)
                    frameBuffer.SetPixel(col+i, row, {string[i]});
            }
    };
}
