#include "tuirenderer.hpp"
#include "rendercommandtype.hpp"
#include "terminalinterface.hpp"
#include "tuitexture.hpp"
#include <cassert>
#include <memory>

using namespace Rendering;

struct TUIRenderer::Impl{ 
    public:
        void Start(TerminalInterface& terminalInterface){
            terminalInterface.EnterAlternateScreen();
            terminalInterface.ClearScreen();

            terminalInterface.GetTerminalSize(row, col);

            diffMap.reserve(col * row);
            for(int i = 0; i < col*row; i++){
                diffMap.push_back(false);
            }
        
            cursorPos = {1, 1};
        
        
            frameBuffer = TUITexture{col, row};
            previousFrameBuffer = TUITexture{col, row};

            started = true;
        }

        void Stop(TerminalInterface& terminalInterface){
            started = false;

            terminalInterface.ExitAlternateScreen();
        }

        void DoCommand(Commands::RenderCommand command){
            command(cursorPos, frameBuffer);
        }

        void Display(TerminalInterface terminalInterface){ 
            assert(started && "Renderer Must be started before displaying.");

            unsigned int x = 0;
            unsigned int y = 0;
            TUITexture::DiffMap(frameBuffer, previousFrameBuffer, diffMap);
            
            terminalInterface.StartSyncOut(stream);
            terminalInterface.HideCursor(stream);
            terminalInterface.MoveCursor(cursorPos.row+1, cursorPos.col+1, stream);
       
            Pixel pixelData;
            for(int i = 0; i < col * row; i++){
                if(diffMap[i]){
                    x = i % frameBuffer.Width();
                    y = i / frameBuffer.Width(); 

                    pixelData = frameBuffer.GetPixel(x, y);

                    terminalInterface.MoveCursor(y+1, x+1, stream);
                    terminalInterface.SetForgroundTrueColor(pixelData.fRed, pixelData.fGreen, pixelData.fBlue, stream);
                    stream << pixelData.character;
                }
            }
        
            terminalInterface.MoveCursor(cursorPos.row+1, cursorPos.col+1, stream);
            terminalInterface.ShowCursor(stream);
            terminalInterface.EndSyncOut(stream);
        
            terminalInterface.InstantStreamToSTDOut(stream);
                        
            TUITexture::CopyChannels(frameBuffer, previousFrameBuffer);
            frameBuffer.Clear();
        }

    private:
        bool started = false;
        CursorPosition cursorPos;
        int row, col;

        std::ostringstream stream;
        std::vector<bool> diffMap{};

        TUITexture previousFrameBuffer;
        TUITexture frameBuffer;
};

TUIRenderer::TUIRenderer() : pImpl(std::make_unique<Impl>()) {}; 

TUIRenderer::~TUIRenderer() = default;

TUIRenderer::TUIRenderer(TUIRenderer&& other) = default;
TUIRenderer& TUIRenderer::operator=(TUIRenderer&& other) = default;

void TUIRenderer::DoCommand(Commands::RenderCommand command){
    pImpl->DoCommand(std::move(command));
}

void TUIRenderer::Start(TerminalInterface& terminalInterface){
    assert(pImpl && "TUI Renderer's Impl is NULL");

    pImpl->Start(terminalInterface);
}

void TUIRenderer::Stop(TerminalInterface& terminalInterface){
    assert(pImpl && "TUI Renderer's Impl is NULL");

    pImpl->Stop(terminalInterface);
}

void TUIRenderer::Display(TerminalInterface& terminalInterface){
    assert(pImpl && "TUI Renderer's Impl is NULL");

    pImpl->Display(terminalInterface);
}
