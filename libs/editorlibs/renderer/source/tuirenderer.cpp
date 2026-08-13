#include "tuirenderer.hpp"
#include <memory>
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

        void DoCommands(Commands::RenderingCommandQueue& queue){
            while(!queue.Empty()){
                queue.PopCommand()(cursorPos, frameBuffer);
            }
        }

        void Display(){ 
            unsigned int x = 0;
            unsigned int y = 0;
            TUITexture::DiffMap(frameBuffer, previousFrameBuffer, diffMap);
        
            terminalController.StartSyncOut(stream);
            terminalController.HideCursor(stream);
            terminalController.MoveCursor(cursorPos.row+1, cursorPos.col+1, stream);
        
            for(int i = 0; i < col * row; i++){
                if(diffMap[i]){
                    x = i % frameBuffer.Width();
                    y = i / frameBuffer.Width();
        
                    terminalController.MoveCursor(y+1, x+1, stream);
                    terminalController.SetForgroundTrueColor(255, 150, 150, stream);
                    stream << frameBuffer.GetPixel(x, y).character;
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

void TUIRenderer::DoCommands(Commands::RenderingCommandQueue& commands){
    pImpl->DoCommands(commands);
}

void TUIRenderer::Display(){
    pImpl->Display();
}
