#include "tuirenderer.hpp"
#include "rendercommandtype.hpp"
#include "tuitexture.hpp"
#include <iostream>
#include <memory>
#include <numbers>
using namespace Rendering;

struct TUIRenderer::Impl{ 
    public:
        Impl(Terminal::TerminalController& terminalController) : terminalController(terminalController){ 
            terminalController.GetTerminalSize(row, col);
        
            frameBuffer = TUITexture{col, row};
            previousFrameBuffer = TUITexture{col, row};
        
            diffMap.reserve(col * row);
            for(int i = 0; i < col*row; i++){
                diffMap.push_back(false);
            }
        
            cursorPos = {1, 1};
        
            terminalController.EnterAlternateScreen();
            terminalController.ClearScreen();
        }; 

        ~Impl(){
            terminalController.ExitAlternteScreen();
        }

        void DoCommand(Commands::RenderCommand command){
            command(cursorPos, frameBuffer);
        }

        void Display(){ 
            unsigned int x = 0;
            unsigned int y = 0;
            TUITexture::DiffMap(frameBuffer, previousFrameBuffer, diffMap);
        
            terminalController.StartSyncOut(stream);
            terminalController.HideCursor(stream);
            terminalController.MoveCursor(cursorPos.row+1, cursorPos.col+1, stream);
       
            Pixel pixelData;
            for(int i = 0; i < col * row; i++){
                if(diffMap[i]){
                    x = i % frameBuffer.Width();
                    y = i / frameBuffer.Width(); 

                    pixelData = frameBuffer.GetPixel(x, y);

                    terminalController.MoveCursor(y+1, x+1, stream);
                    terminalController.SetForgroundTrueColor(pixelData.fRed, pixelData.fGreen, pixelData.fBlue, stream);
                    stream << pixelData.character;
                }
            }
        
            terminalController.MoveCursor(cursorPos.row+1, cursorPos.col+1, stream);
            terminalController.ShowCursor(stream);
            terminalController.EndSyncOut(stream);
        
            terminalController.InstantStreamToSTDOut(stream);
        
            stream.clear();
            stream.str("");
                        
            TUITexture::CopyChannels(frameBuffer, previousFrameBuffer);
            frameBuffer.Clear();
        }

    private:
        Terminal::TerminalController& terminalController;

        CursorPosition cursorPos;
        int row, col;

        std::ostringstream stream;
        std::vector<bool> diffMap{};

        TUITexture previousFrameBuffer;
        TUITexture frameBuffer;
};

TUIRenderer::TUIRenderer(Terminal::TerminalController& terminalController) 
    : pImpl(std::make_unique<Impl>(terminalController)) {}; 

TUIRenderer::~TUIRenderer() = default;

void TUIRenderer::DoCommand(Commands::RenderCommand command){
    pImpl->DoCommand(std::move(command));
}

void TUIRenderer::Display(){
    pImpl->Display();
}
