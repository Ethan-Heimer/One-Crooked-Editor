#pragma once

#include "rendercommand.hpp"
#include "tuitexture.hpp"

#include <asm-generic/ioctls.h>
#include <cstdio>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/ioctl.h>
#include <vector>

namespace Rendering{
    constexpr const std::string ENTER_ALTERNATE = "\e[?1049h";
    constexpr const std::string EXIT_ALTERNATE = "\e[?1049l";
    constexpr const std::string HIDE_CURSOR = "\e[?25l";
    constexpr const std::string SHOW_CURSOR = "\e[?25h";
    constexpr const std::string CLEAR = "\e[2J";
    constexpr const std::string DECSET = "\e[?2026h";
    constexpr const std::string DECRST = "\e[?2026l";

    class TUIRenderer{
        public:
            unsigned int row, col;
            int num = 0;
            std::stringstream stream;
            std::vector<bool> diffMap{};

            TUIRenderer(){ 
                struct winsize winSize; 
                if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &winSize) == 0){
                   row = winSize.ws_row;
                   col = winSize.ws_col;
                } else {
                    //throw somthing
                }

                frameBuffer = TUITexture{col, row};
                previousFrameBuffer = TUITexture{col, row};

                diffMap.reserve(col * row);
                for(unsigned int i = 0; i < col*row; i++){
                    diffMap.push_back(false);
                }

                std::cout << ENTER_ALTERNATE;
                std::cout << CLEAR;
            }; 

            ~TUIRenderer(){
                std::cout << EXIT_ALTERNATE;
            };

            TUIRenderer(const TUIRenderer& other) = delete;
            TUIRenderer(TUIRenderer&& other) = default;

            void DoCommand(Commands::RenderCommand& command){
                command.Do(frameBuffer);
            }

            void Display(unsigned int ms){
                std::string msstr = std::to_string(ms);
                for(size_t i = 0; i < msstr.length(); i++){
                    frameBuffer.SetPixel(i+20, 30, {msstr[i]});
                }

                unsigned int x = 0;
                unsigned int y = 0;
                TUITexture::DiffMap(frameBuffer, previousFrameBuffer, diffMap);

                stream << DECSET;
                stream << HIDE_CURSOR << "\e[1;1H";
                for(unsigned int i = 0; i < col * row; i++){
                    if(diffMap[i]){
                        x = i % frameBuffer.Width();
                        y = i / frameBuffer.Width();

                        stream << std::format("\e[{};{}H\e[38;2;200;100;100m{}", y+1, x+1, frameBuffer.GetPixel(x, y).character);
                    }
                }
                stream << "\e[1;1H" << SHOW_CURSOR;
                stream << DECRST;

                fputs(stream.str().c_str(), stdout);
                fflush(stdout);

                stream.clear();
                stream.str("");
                
                TUITexture::CopyChannels(frameBuffer, previousFrameBuffer);
                frameBuffer.Clear();
            }

        private:
            TUITexture previousFrameBuffer;
            TUITexture frameBuffer;
    };
}
