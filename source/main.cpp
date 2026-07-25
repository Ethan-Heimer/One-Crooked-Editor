#include <memory>

#include <ncurses.h>
#include <queue>

#include "bufferfilehandler.h"
#include "editorcontext.h"
#include "editorfilehandling/ieditorfilehandler.h"
#include "ieditable.h"

#include "iinputmanager.h"

#include "inputmanager.h"

using namespace std;
using namespace Systems::Input;
using namespace Editor;
using namespace Editor::States;
using namespace Buffers;

void InitScreen();
void KillScreen();

void UpdateUI(shared_ptr<IEditable> buffer, int& lineOffset, int& colOffset);

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    queue<int> inputQueue;
    string fileName{argv[1]};
 
    EditorContext context{BufferFileInterpreter{}, &inputQueue, fileName};

    shared_ptr<IInputManager> inputManager = std::make_shared<InputManager>();
    InitScreen();

    int lineOffset = 0;
    int colOffset = 0;

    while(!context.quit){
        int ch = inputManager->GetKeyInput();
        if(ch != ERR){
            if(ch == KEY_BACKSPACE)
                inputQueue.push('\b');
            else
                inputQueue.push(ch);
        }
         
        context.Update();
        UpdateUI(context.buffer, lineOffset, colOffset);
    }

    KillScreen();
}

void UpdateUI(shared_ptr<IEditable> buffer, int& lineOffset, int& colOffset){ 
        const int lineColWidth = 3;
        int currentLineNumber = buffer->GetCurrentLineNumber();
        int currentCursorCol = buffer->GetCursorX();
        int row, col;

        getmaxyx(stdscr, row, col);

        erase();        

        auto start = buffer->BeginAtCurrentLine();
        auto end = buffer->EndStepsFromCurrentLine(row-1);

        for(auto line = start ; line != end; ++line){
            if((*line).length() == 0)
                continue;

            printw(" %*d| %s \n", 
                    lineColWidth,
                    0,
                    (*line).substr(colOffset, colOffset + col - 6 - lineColWidth).c_str()); //(colOffset, colOffset + col - 6 - lineColWidth).c_str());
        }

        move(0, currentCursorCol - colOffset + lineColWidth+3);
        refresh();
        
        while(currentLineNumber - lineOffset >= row - 5)
            lineOffset++;

        while(currentLineNumber < lineOffset)
            lineOffset--;

        while(currentCursorCol - colOffset > col - 10){
            colOffset++;
        }

        while(currentCursorCol < colOffset)
            colOffset--;
}

void InitScreen(){
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(1);
    nonl();
    scrollok(stdscr, FALSE);
    idlok(stdscr, FALSE);
}

void KillScreen(){
    endwin();
}


