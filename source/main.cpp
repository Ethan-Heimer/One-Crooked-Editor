#include <memory>

#include <iostream>
#include <ncurses.h>
#include <fstream>

#include "gapbuffer/gapbuffer.h"
#include "linkedlist/doublylinkedlist.h"

#define ctrl(x) ((x) & 0x1f)

using namespace std;
using Node = shared_ptr<DoublyLinkedList<GapBuffer>::Node>;

struct Point{
    int x = 0;
    int y = 0;
};

void SetCursorY(Node& line, Point& point, int Y){
    int differance = Y - point.y;

    for(int i = 0; i < abs(differance); i++){
        if(differance < 0){
            if(line->previous.lock()){
                line = line->previous.lock();
                point.y --;
            }
            else
                break;
        }
        else{
            if(line->next){
                line = line->next;
                point.y ++;
            }
            else
                break;
        }

    }
}

void SetCursorX(Node& line, Point& point, int X){
    point.x = X;
    line->data->MoveGapTo(X);
}

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    const string fileName{argv[1]};
    ifstream inputFile{fileName};

    DoublyLinkedList<GapBuffer> lines;
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
    Point curserPosition;
    int lineOffset = 0;
    int colOffset = 0;

    while(!quit){
        int row, col;
        getmaxyx(stdscr, row, col);

        erase();
        Node currentNode = lines.head;
        for(int i = 0; i < lineOffset && currentNode; i++){
            currentNode = currentNode->next; 
        }
        
        for(int i = lineOffset; i < (row + lineOffset) && currentNode; i++){
            printw("%s \n", currentNode->data->Substring(colOffset, colOffset + col - 2).c_str());
            currentNode = currentNode->next;
        }

        if(curserPosition.y >= row - 5 + lineOffset)
            lineOffset++;

        if(curserPosition.y >= 5 && curserPosition.y < lineOffset + 5)
            lineOffset--;

        if(currentLine->data->GetGapIndex() >= col-2 - 5 + colOffset)
            colOffset++;

        if(currentLine->data->GetGapIndex() >= 5 && currentLine->data->GetGapIndex() < colOffset + 5)
            colOffset--;

        move(curserPosition.y - lineOffset, currentLine->data->GetGapIndex() - colOffset);
        refresh();

        int input = getch();

        if(input == ERR)
            continue;

        else if(input == ctrl('w'))
            quit = true;
        else if(input == KEY_DOWN){
            SetCursorY(currentLine, curserPosition, curserPosition.y+1);
        }
        
        else if(input == KEY_UP){
            SetCursorY(currentLine, curserPosition, curserPosition.y-1);
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
                    SetCursorY(currentLine, curserPosition, curserPosition.y-1); 
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
                SetCursorY(currentLine, curserPosition, curserPosition.y+1);
                currentLine->data->Insert(substring);
                currentLine->data->MoveGapTo(0);
            }
        }
        else
            currentLine->data->Insert(input);
    }

    //saving
    ofstream saveFile{fileName};
    if(saveFile.is_open()){
        for(auto node : lines){
            saveFile << ((string)*node).c_str() << endl;
        }
        saveFile.close();
    }

    endwin();
}

//END OF THE FILE WHYYYY
//hhh
