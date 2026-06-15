#include <memory>

#include <iostream>
#include <ncurses.h>
#include <fstream>

#include "gapbuffer/gapbuffer.h"
#include "linkedlist/doublyindexedlinkedlist.h"

#define ctrl(x) ((x) & 0x1f)

using namespace std;
using Node = shared_ptr<DoublyIndexedLinkedList<GapBuffer>::Node>;

void Save(DoublyIndexedLinkedList<GapBuffer>& lines, const string& fileName){
    ofstream saveFile{fileName};
    if(saveFile.is_open()){
        for(auto node : lines){
            saveFile << ((string)*node).c_str() << endl;
        }
        saveFile.close();
    }
}

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    const string fileName{argv[1]};
    ifstream inputFile{fileName};

    DoublyIndexedLinkedList<GapBuffer> lines;
    Node currentLine;
    string line;

    if(inputFile.is_open()){
        while(getline(inputFile, line)){
            lines.Append(line, 5);
        }

        inputFile.close();
    }

    if(lines.head)
        currentLine = lines.head;
    else{
        lines.Append("", 5);    
        currentLine = lines.head;
    }

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

    bool quit = false;
    int lineOffset = 0;
    int colOffset = 0;

    while(!quit){
        const int lineColWidth = 3;
        int row, col;
        getmaxyx(stdscr, row, col);

        erase();
        
        Node currentNode = currentLine;
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
        
        int currentLineNumber = currentLine->index;
        int currentCursorCol = currentLine->data->GetGapIndex();

        while(currentLineNumber - lineOffset >= row - 5)
            lineOffset++;

        if(currentLineNumber < lineOffset)
            lineOffset--;

        while(currentCursorCol - colOffset > col - 10){
            colOffset++;
        }

        while(currentCursorCol < colOffset)
            colOffset--;

        move(currentLineNumber - lineOffset, currentCursorCol - colOffset + lineColWidth+3);
        refresh();

        int input = getch();

        if(input == ERR)
            continue;

        else if(input == ctrl('x'))
            quit = true;
        else if(input == ctrl('w'))
            Save(lines, fileName);
        else if(input == ctrl('X')){
           quit = true; 
           Save(lines, fileName);
        }
        else if(input == KEY_DOWN){
            if(currentLine->next)
                currentLine = currentLine->next;
        } 
        else if(input == KEY_UP){
            if(currentLine->previous.lock())
                currentLine = currentLine->previous.lock();
        }
        else if(input == KEY_LEFT){
            currentLine->data->MoveGapLeft();
        }
        else if(input == KEY_RIGHT){
            currentLine->data->MoveGapRight();
        }
        else if(input == KEY_BACKSPACE){
            bool isGapAtBeginning = currentLine->data->IsGapAtBeginning();
            if(isGapAtBeginning){
                Node previousLine = currentLine->previous.lock();
                if(previousLine){
                    string data = currentLine->data->ToString();
                    previousLine->data->Insert(data);

                    lines.Remove(currentLine);
                    if(currentLine->previous.lock())
                        currentLine = currentLine->previous.lock();
                }
            }
            else
                currentLine->data->Delete();
        } 
        else if(input == '\n' || input == '\r'){
            lines.AppendAfter(currentLine, "", 5);
            int gapIndex = currentLine->data->GetGapIndex();
            int endIndex = currentLine->data->BufferSize();
            string substring = currentLine->data->Substring(gapIndex, endIndex);
            currentLine->data->DeleteBetween(gapIndex, endIndex);

            if(currentLine->next){
                currentLine = currentLine->next;
                currentLine->data->Insert(substring);
                currentLine->data->MoveGapTo(0);
            }
        }
        else
            currentLine->data->Insert(input);
    }

    endwin();
}
