#include "terminalenviorment.hpp"
#include <sstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <utility>

#include "terminalenviormentevents.hpp"

using namespace CrookedEditor::Runtime;

constexpr static std::string ENTER_ALTERNATE = "\e[?1049h";
constexpr static std::string EXIT_ALTERNATE = "\e[?1049l";
constexpr static std::string HIDE_CURSOR = "\e[?25l";
constexpr static std::string SHOW_CURSOR = "\e[?25h";
constexpr static std::string CLEAR = "\e[2J";
constexpr static std::string DECSET = "\e[?2026h";
constexpr static std::string DECRST = "\e[?2026l";

static struct termios startingState;
static bool setStart = false;

TerminalEnviorment::TerminalEnviorment(Events::RequestBus& requestBus)
    : requestBus(requestBus){
    if(!setStart){
        tcgetattr(STDIN_FILENO, &startingState);
        setStart = true;
    }

    struct winsize winSize;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &winSize) != 0){
        throw;
    }

    if(winSize.ws_row == 0 || winSize.ws_col == 0){
        throw;
    }

    requestBus.AddRequest<API::TermGetSize>([&](){
       return GetTerminalSize();
    });

    requestBus.AddRequest<API::TermGetStartSyncOutEsc>([&](){
        return DECSET;
    });

    requestBus.AddRequest<API::TermGetEndSyncOutEsc>([&](){
        return DECRST;
    });

    requestBus.AddRequest<API::TermGetHideCursorEsc>([&](){
        return HIDE_CURSOR;
    });

    requestBus.AddRequest<API::TermGetShowCursorEsc>([&](){
        return SHOW_CURSOR;
    });

    requestBus.AddRequest<API::TermGetMoveCursorEsc>([&](int row, int col){
        return std::format("\e[{};{}H", row, col);
    });

    requestBus.AddRequest<API::TermGetForgroundColorEsc>([&](unsigned char r, unsigned char g, unsigned char b){
        return std::format("\e[38;2;{};{};{}m", r, g, b);
    });

    requestBus.AddRequest<API::TermDisplayStream>([&](const std::ostringstream& stream){
        InstantStreamToSTDOut(stream);
    });

    requestBus.AddRequest<API::TermEnterRawMode>([&](){
        EnterRawMode();
    });

    requestBus.AddRequest<API::TermExitRawMode>([&](){
        ExitRawMode();
    });

    requestBus.AddRequest<API::TermEnterAlternateScreen>([&](){
        EnterAlternateScreen();
    });

    requestBus.AddRequest<API::TermExitAlternateScreen>([&](){
        ExitAlternteScreen();
    });

    requestBus.AddRequest<API::TermHideCursor>([&](){
        HideCursor();
    });

    requestBus.AddRequest<API::TermShowCursor>([&](){
        ShowCursor();
    });

    requestBus.AddRequest<API::TermMoveCursor>([&](int x, int y){
        MoveCursor(x, y);
    });

    requestBus.AddRequest<API::TermClearScreen>([&](){
        ClearScreen();
    });
}

TerminalEnviorment::~TerminalEnviorment(){
    ExitRawMode();
    ExitAlternteScreen();
}

std::pair<int, int> TerminalEnviorment::GetTerminalSize() const{
    struct winsize winSize;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &winSize) != 0){
        throw;
    }
    int row = winSize.ws_row;
    int col = winSize.ws_col;
    
    return std::pair<int, int>{row, col};
}

void TerminalEnviorment::EnterRawMode(){
    struct termios raw = startingState;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void TerminalEnviorment::EnterRawModeFD(int fd){
    struct termios raw;
    tcgetattr(fd, &startingState);

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(fd, TCSAFLUSH, &raw);
}

void TerminalEnviorment::ExitRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &startingState);
}

void TerminalEnviorment::EnterAlternateScreen(){
    std::cout << ENTER_ALTERNATE;
}

void TerminalEnviorment::ExitAlternteScreen(){     
    std::cout << EXIT_ALTERNATE;
}

void TerminalEnviorment::ClearScreen(){
    std::cout << CLEAR;
}
 
void TerminalEnviorment::HideCursor(){
    std::cout << HIDE_CURSOR;
}

void TerminalEnviorment::ShowCursor(){
    std::cout << SHOW_CURSOR;
}

void TerminalEnviorment::MoveCursor(int row, int col){
    std::cout << std::format("\e[{};{}H", row, col);
}

void TerminalEnviorment::InstantStreamToSTDOut(const std::ostringstream& stream){
    std::cout << stream.str() << std::flush;
}
