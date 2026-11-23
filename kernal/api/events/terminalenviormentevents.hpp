#pragma once

#include "requestbus.hpp"
#include <utility>

namespace CrookedEditor::API{
    struct TermGetSize : public Events::IRequest<std::pair<int, int>>{};

    struct TermGetStartSyncOutEsc : public Events::IRequest<std::string>{};
    struct TermGetEndSyncOutEsc : public Events::IRequest<std::string>{};
    struct TermGetHideCursorEsc : public Events::IRequest<std::string>{};
    struct TermGetShowCursorEsc : public Events::IRequest<std::string>{};
    struct TermGetMoveCursorEsc : public Events::IRequest<std::string, int, int>{};
    struct TermGetForgroundColorEsc 
        : public Events::IRequest<std::string, unsigned char, unsigned char, unsigned char>{};

    struct TermDisplayStream : public Events::IEvent<std::ostringstream>{};
    struct TermMoveCursor : public Events::IEvent<int, int>{};

    struct TermEnterRawMode : public Events::INotif{};
    struct TermExitRawMode : public Events::INotif{};
    
    struct TermHideCursor : public Events::INotif{};
    struct TermShowCursor : public Events::INotif{};

    struct TermEnterAlternateScreen : public Events::INotif{};
    struct TermExitAlternateScreen: public Events::INotif{};

    struct TermClearScreen : public Events::INotif{};
}
