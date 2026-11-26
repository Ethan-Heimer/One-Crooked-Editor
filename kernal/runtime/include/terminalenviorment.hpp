#pragma once

#include "requestbus.hpp"
#include <termios.h>

namespace CrookedEditor::Runtime{
    class TerminalEnviorment final{
        public:
            TerminalEnviorment(Events::RequestBus& requestBus);
            ~TerminalEnviorment();

            std::pair<int, int> GetTerminalSize() const;

            void EnterRawMode();

            void EnterRawModeFD(int fd);

            void ExitRawMode();

            void EnterAlternateScreen();

            void ExitAlternteScreen();

            void ClearScreen();

            void HideCursor();

            void ShowCursor();

            void MoveCursor(int row, int col);

            void InstantStreamToSTDOut(const std::ostringstream& stream);

        private:
            Events::RequestBus& requestBus;
    };
}
