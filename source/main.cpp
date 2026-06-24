#include <memory>

#include <iostream>
#include <ncurses.h>
#include <fstream>

#include "editor.h"
#include "context.h"
#include "gapbuffer/gapbuffer.h"
#include "icontext.h"
#include "ieditor.h"
#include "iinputmanager.h"
#include "linkedlist/doublyindexedlinkedlist.h"

#include "inputmanager.h"

using namespace std;
using namespace Systems::Input;

using Lines = DoublyIndexedLinkedList<GapBuffer>&;
using Line = shared_ptr<DoublyIndexedLinkedList<GapBuffer>::Node>;

void InitScreen();
void KillScreen();

void UpdateUI(Line& currentLine, int& lineOffset, int& colOffset);

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    std::shared_ptr<IInputManager> inputManager = std::make_shared<InputManager>();
    string fileName{argv[1]};

    std::shared_ptr<Editor::States::StateContextFactory> stateContextFactory 
        = std::make_shared<Editor::States::StateContextFactory>();

    std::shared_ptr<Editor::EditorFactory> editorFactory = std::make_shared<Editor::EditorFactory>();

    std::shared_ptr<Editor::IEditor> editor 
        = editorFactory->Instanciate<Editor::Editor>(inputManager, fileName, stateContextFactory);

    InitScreen();

    int lineOffset = 0;
    int colOffset = 0;

    while(!editor->quit){
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
