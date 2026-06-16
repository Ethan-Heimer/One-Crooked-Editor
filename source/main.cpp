#include <memory>

#include <iostream>
#include <ncurses.h>
#include <fstream>

#include "gapbuffer/gapbuffer.h"
#include "linkedlist/doublyindexedlinkedlist.h"

#define ctrl(x) ((x) & 0x1f)

using namespace std;
using Lines = DoublyIndexedLinkedList<GapBuffer>&;
using Line = shared_ptr<DoublyIndexedLinkedList<GapBuffer>::Node>;

void InitScreen();
void KillScreen();

void LoadFile(string filenName, Lines& lines, Line& currentLine);

int GetInput();

void GotoNextLine(Line& currentLine);
void GotoPreviousLine(Line& currentLine);

void MoveCursorLeft(Line& currentLine);
void MoveCursorRight(Line& currentLine);

bool IsCursorAtBeginningOfLine(Line& currentLine);

void DeleteCharacter(Line& currentLine);
void InsertCharacter(Line& currentLine, char input);

void DeleteLine(Lines& lines, Line& currentLine);
Line InsertLine(Lines& lines, Line& currentLine);

void AppendTextToNextLine(Line& currentLine);

void ExecuteCommands(const int& input, bool& quitToken, Lines& lines, 
        Line& currentLine, const string& fileName);

void UpdateUI(Line& currentLine, int& lineOffset, int& colOffset);
void Save(Lines& lines, const string& fileName);

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    DoublyIndexedLinkedList<GapBuffer> lines;
    Line currentLine;
    string fileName{argv[1]};
    LoadFile(fileName, lines, currentLine);

    InitScreen();

    bool quit = false;
    int lineOffset = 0;
    int colOffset = 0;
    while(!quit){
        int input = GetInput();
        UpdateUI(currentLine, lineOffset, colOffset);
        ExecuteCommands(input, quit, lines, currentLine, fileName);
    }

    KillScreen();
}

void ExecuteCommands(const int& input, bool& quitToken, Lines& lines, Line& currentLine, const string& fileName){
    if(input == ERR)
        return;

    if(input == ctrl('x'))
        quitToken = true;
    else if(input == ctrl('w'))
        Save(lines, fileName);
    else if(input == ctrl('X')){
        quitToken = true; 
        Save(lines, fileName);
    }
    else if(input == KEY_DOWN){
        GotoNextLine(currentLine);
    } 
    else if(input == KEY_UP){
        GotoPreviousLine(currentLine);
    }
    else if(input == KEY_LEFT){
        MoveCursorLeft(currentLine);
    }
    else if(input == KEY_RIGHT){
        MoveCursorRight(currentLine);
    }
    else if(input == KEY_BACKSPACE){
        if(IsCursorAtBeginningOfLine(currentLine)){
            DeleteLine(lines, currentLine);
        }
        else
            DeleteCharacter(currentLine);
    } 
    else if(input == '\n' || input == '\r'){
        InsertLine(lines, currentLine);
        AppendTextToNextLine(currentLine);
    }
    else
        InsertCharacter(currentLine, input);

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

void LoadFile(string fileName, Lines& lines, Line& currentLine){
    ifstream inputFile{fileName};

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

void GotoNextLine(Line& currentLine){
    if(currentLine->next)
        currentLine = currentLine->next;
}

void GotoPreviousLine(Line& currentLine){
    if(currentLine->previous.lock())
        currentLine = currentLine->previous.lock();
}

void MoveCursorLeft(Line& currentLine){
    currentLine->data->MoveGapLeft();
}

void MoveCursorRight(Line& currentLine){
    currentLine->data->MoveGapRight();
}

bool IsCursorAtBeginningOfLine(Line& currentLine){
    return currentLine->data->IsGapAtBeginning();
}

void InsertCharacter(Line& currentLine, char character){
    currentLine->data->Insert(character);
}

void DeleteCharacter(Line& currentLine){
    currentLine->data->Delete();
}

Line InsertLine(Lines& lines, Line& currentLine){
    return lines.AppendAfter(currentLine, "", 5);
}

void DeleteLine(Lines& lines, Line& currentLine){
    Line previousLine = currentLine->previous.lock();
    if(previousLine){
        string data = currentLine->data->ToString();
        previousLine->data->Insert(data);

        lines.Remove(currentLine);
        if(currentLine->previous.lock())
            currentLine = currentLine->previous.lock();
    }
}

void AppendTextToNextLine(Line& currentLine){
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

void Save(DoublyIndexedLinkedList<GapBuffer>& lines, const string& fileName){
    ofstream saveFile{fileName};
    if(saveFile.is_open()){
        for(auto node : lines){
            saveFile << ((string)*node).c_str() << endl;
        }
        saveFile.close();
    }
}

int GetInput(){
    return getch();
}
