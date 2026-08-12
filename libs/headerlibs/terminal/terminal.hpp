#pragma once

#include <cstdio>
#include <format>
#include <sstream>
#include <sys/ioctl.h>
#include <iostream>
#include <ostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#include <string>

namespace Terminal{
    constexpr const std::string ENTER_ALTERNATE = "\e[?1049h";
    constexpr const std::string EXIT_ALTERNATE = "\e[?1049l";
    constexpr const std::string HIDE_CURSOR = "\e[?25l";
    constexpr const std::string SHOW_CURSOR = "\e[?25h";
    constexpr const std::string CLEAR = "\e[2J";
    constexpr const std::string DECSET = "\e[?2026h";
    constexpr const std::string DECRST = "\e[?2026l";
    
    class TerminalController final{
        public:
            TerminalController(){
                tcgetattr(STDIN_FILENO, &startingState);
                if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &winSize) != 0){
                    throw;
                }

                if(winSize.ws_row == 0 || winSize.ws_col == 0){
                    throw;
                }
            }

            ~TerminalController(){
                tcsetattr(STDIN_FILENO, TCSAFLUSH, &startingState);
                ExitAlternteScreen();
            }


            void GetTerminalSize(unsigned int& row, unsigned int& col) const{
                row = winSize.ws_row;
                col = winSize.ws_col;
            }

            void EnterRawMode(){
                struct termios raw = startingState;
                raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
                raw.c_oflag &= ~(OPOST);
                raw.c_cflag &= ~(CS8);
                raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
                raw.c_cc[VMIN] = 0;
                raw.c_cc[VTIME] = 0;
                tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
            }

            void EnterAlternateScreen(){
                std::cout << ENTER_ALTERNATE;
            }

            void ExitAlternteScreen(){     
                std::cout << EXIT_ALTERNATE;
            }

            void ClearScreen(){
                std::cout << CLEAR;
            }

            void StartSyncOut(std::ostream &stream){
                stream << DECSET;
            }
            
            void EndSyncOut(std::ostream& stream){
                stream << DECRST;
            }

            void StartSyncOut(){
                StartSyncOut(std::cout);
            }
            
            void EndSyncOut(){
                EndSyncOut(std::cout);
            }

            void HideCursor(std::ostream& stream){
                stream << HIDE_CURSOR;
            }

            void ShowCursor(std::ostream& stream){
                stream << SHOW_CURSOR;
            }

            void HideCursor(){
                HideCursor(std::cout);
            }

            void ShowCursor(){
                ShowCursor(std::cout);
            }

            constexpr void MoveCursor(int row, int col, std::ostream& stream){
                stream << "\e[" << row << ";" << col << "H";
            }

            constexpr void MoveCursor(int row, int col){
                MoveCursor(row, col, std::cout);
            }

            constexpr void SetForgroundTrueColor(unsigned char r, unsigned char g, unsigned char b, std::ostream& stream){
                stream << std::format("\e[38;2;{};{};{}m", r, g, b);
            }

            void SetForgroundTrueColor(unsigned char r, unsigned char g, unsigned char b){
                SetForgroundTrueColor(r, g, b, std::cout);
            }

            void InstantStreamToSTDOut(std::ostringstream& stream){
                fputs(stream.str().c_str(), stdout);
                fflush(stdout);
            }

        private:
            struct winsize winSize;
            struct termios startingState;
    };}
