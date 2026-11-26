#pragma once

#include <functional>
#include <ostream>
#include <sstream>

namespace Rendering{
    struct TerminalInterface{
        std::function<void()> ClearScreen;

        std::function<void(int& row, int& col)> GetTerminalSize;

        std::function<void()> EnterAlternateScreen;
        std::function<void()> ExitAlternateScreen;

        std::function<void(std::ostream& stream)> StartSyncOut;
        std::function<void(std::ostream& stream)> EndSyncOut;

        std::function<void(std::ostream& stream)> ShowCursor;
        std::function<void(std::ostream& stream)> HideCursor;

        std::function<void(unsigned char r, unsigned char g, unsigned char b, std::ostream& stream)> SetForgroundTrueColor;

        std::function<void(int row, int col, std::ostream& stream)> MoveCursor;
        std::function<void(std::ostringstream& stream)> InstantStreamToSTDOut;
    };
}
