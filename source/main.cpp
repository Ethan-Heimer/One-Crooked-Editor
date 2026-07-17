#include <memory>

#include <ncurses.h>
#include <queue>

#include "buffer.h"
#include "metaprogramming/editormeta.h"

#include "editorstates.h"
#include "gapbuffer.h"
#include "iinputmanager.h"
#include "doublyindexedlinkedlist.h"
#include <iostream>

#include "inputmanager.h"

using namespace std;
using namespace Systems::Input;
using namespace Editor;
using namespace Editor::States;
using namespace Metaprogramming;

using Lines = DoublyIndexedLinkedList<GapBuffer>&;
using Line = shared_ptr<DoublyIndexedLinkedList<GapBuffer>::Node>;

void InitScreen();
void KillScreen();

void UpdateUI(Line& currentLine, int& lineOffset, int& colOffset);

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    queue<int> inputQueue;
    shared_ptr<IInputManager> inputManager = std::make_shared<InputManager>();
    string fileName{argv[1]};

    auto editorFactory = DefaultEditorFactory<Buffers::Buffer, NormalState, InsertState>();
    auto editor = editorFactory.Instanciate(&inputQueue, fileName);

    InitScreen();

    int lineOffset = 0;
    int colOffset = 0;

    while(!editor->quit){
        int ch = inputManager->GetKeyInput();
        if(ch != ERR){
            if(ch == KEY_BACKSPACE)
                inputQueue.push('\b');
            else
                inputQueue.push(ch);
        }
         
        editor->Update();
        UpdateUI(editor->buffer->currentLine, lineOffset, colOffset);
    }

    KillScreen();
}

void UpdateUI(Line& currentLine, int& lineOffset, int& colOffset){ 
        const int lineColWidth = 3;
        int currentLineNumber = currentLine->index;
        int currentCursorCol = currentLine->data->GetGapIndex();
        int row, col;

        getmaxyx(stdscr, row, col);

        erase();        
        Line currentNode = currentLine;
        while(currentNode && currentNode->index != lineOffset){
            currentNode = currentNode->previous.lock();   
        }

        for(int i = 0; i < row && currentNode; i++){
            printw(" %*d| %s \n", 
                    lineColWidth,
                    currentNode->index+1,
                    currentNode->data->Substring(colOffset, colOffset + col - 6 - lineColWidth).c_str());

            currentNode = currentNode->next;
        }
        move(currentLineNumber - lineOffset, currentCursorCol - colOffset + lineColWidth+3);
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


