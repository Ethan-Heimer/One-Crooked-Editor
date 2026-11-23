#include "terminalinterfacehelper.hpp"
#include "terminalenviormentevents.hpp"
#include "terminalinterface.hpp"

#include <ostream>

namespace CrookedEditor::Renderer{
    Rendering::TerminalInterface MakeTerminalInterface(Events::RequestBus& requestBus){
        Rendering::TerminalInterface interface;
        
        interface.ClearScreen = [&](){ 
            requestBus.SendRequest<API::TermClearScreen>();
        };
    
        interface.GetTerminalSize = [&](int& row, int& col){ 
            auto response = requestBus.SendRequest<API::TermGetSize>();
            if(response.has_value()){
                row = response.value().first;
                col = response.value().second;
            }
        };
    
        interface.EnterAlternateScreen = [&](){ 
            requestBus.SendRequest<API::TermEnterAlternateScreen>();
        };
        interface.ExitAlternateScreen = [&](){ 
            requestBus.SendRequest<API::TermExitAlternateScreen>();
        };
    
        interface.StartSyncOut = [&](std::ostream& stream){ 
            auto response = requestBus.SendRequest<API::TermGetStartSyncOutEsc>();
            if(response.has_value()){
                stream << response.value();
            }
        };

        interface.EndSyncOut = [&](std::ostream& stream){ 
            auto response = requestBus.SendRequest<API::TermGetEndSyncOutEsc>();
            if(response.has_value()){
                stream << response.value();
            }
        };
    
        interface.InstantStreamToSTDOut = [&](std::ostringstream& stream){
            requestBus.SendRequest<API::TermDisplayStream>(stream);
            stream.str("");
        };
    
        interface.ShowCursor = [&](std::ostream& stream){ 
            auto response = requestBus.SendRequest<API::TermGetShowCursorEsc>();
            if(response.has_value()){
                stream << response.value();
            }
        };

        interface.HideCursor = [&](std::ostream& stream){ 
            auto response = requestBus.SendRequest<API::TermGetHideCursorEsc>();
            if(response.has_value()){
                stream << response.value();
            }
        };
    
        interface.MoveCursor = [&](int row, int col, std::ostream& stream){ 
            auto response = requestBus.SendRequest<API::TermGetMoveCursorEsc>(row, col);
            if(response.has_value()){
                stream << response.value();
            }
        };
        interface.SetForgroundTrueColor = [&](unsigned char r, unsigned char g, unsigned char b, std::ostream& stream){
            auto response = requestBus.SendRequest<API::TermGetForgroundColorEsc>(r, g, b);
            if(response.has_value()){
                stream << response.value();
            }
        };
    
        return interface;
    }
}
