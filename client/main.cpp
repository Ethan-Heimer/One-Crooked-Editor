#include <memory>

#include <ncurses.h>
#include <queue>
#include <chrono>

#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>

#include "bufferfilehandler.h"
#include "editorcontext.h"
#include "editorstates.h"
#include "ieditable.h"

#include "iinputmanager.h"

#include "inputmanager.h"

using namespace std;
using namespace std::chrono;
using namespace Editor;

using namespace Systems::Input;
using namespace Editor::States;

using namespace CrookedEditor::Buffers;
using namespace CrookedEditor::States;

void InitScreen();
void KillScreen();

void process_mem_usage(double& vm_usage, double& resident_set);
void UpdateUI(shared_ptr<IEditable> buffer, uint64_t ms, double vm, double rss, const std::string& mode, int& lineOffset, int& colOffset);

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    queue<int> inputQueue;
    string fileName{argv[1]};
 
    EditorContext context{BufferFileInterpreter{}, 
        DefaultStates<NormalState, InsertState>{},
        &inputQueue, fileName};

    shared_ptr<IInputManager> inputManager = std::make_shared<InputManager>();
    InitScreen();

    int lineOffset = 0;
    int colOffset = 0;

    milliseconds msd {};
    double vm, rss;

    while(!context.quit){
        milliseconds msb = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        int ch = inputManager->GetKeyInput();
        if(ch != ERR){
            if(ch == KEY_BACKSPACE)
                inputQueue.push('\b');
            else
                inputQueue.push(ch);
        }
         
        context.Update();
        UpdateUI(context.buffer, msd.count(), vm, rss, context.CurrentModeName(), lineOffset, colOffset);

        milliseconds msa = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        msd = msa-msb;

        process_mem_usage(vm, rss);
    }

    KillScreen();
}

void UpdateUI(shared_ptr<IEditable> buffer, uint64_t ms, double vm, double rss, const std::string& mode, int& lineOffset, int& colOffset){ 
        const int lineColWidth = 3;
        int currentLineNumber = buffer->GetCurrentLineNumber();
        int currentCursorCol = buffer->GetCursorX();
        int row, col;

        getmaxyx(stdscr, row, col);

        erase();        

        auto start = buffer->BeginStepsFromCurrentLine(-5);
        auto end = buffer->EndStepsFromCurrentLine(row-1);

        int linenum = start.LineNumber();
        int termLine = 0;
        for(auto line = start ; line != end; ++line){
            /* throw away to stop overline wrapping */
            move(termLine, 0);
            clrtoeol();
            termLine ++;

            if((*line).length() == 0 || colOffset > (*line).length()){ 
                printw(" %*d| %s \n", lineColWidth, linenum, "");
                linenum++;
                continue;
            }

            printw(" %*d| %s \n", 
                    lineColWidth,
                    linenum,
                    (*line).substr(colOffset, colOffset + col - 5 - lineColWidth).c_str()); //(colOffset, colOffset + col - 6 - lineColWidth).c_str());
                linenum++;

        }

        int offset = currentLineNumber <= 5 ? currentLineNumber : 5;
        move(row-1, 0);
        clrtoeol();
        printw("mode: [%s] @ ms: %lu | vm: %lf KB (%lf MB) rss: %lf", mode.c_str(), ms, vm, vm/1024, rss);

        move(offset, currentCursorCol - colOffset + lineColWidth+3);
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
    set_escdelay(0);
}

void KillScreen(){
    endwin();
}

// Source - https://stackoverflow.com/a/671389
// Posted by Don Wakefield, modified by community. See post 'Timeline' for change history
// Retrieved 2026-08-05, License - CC BY-SA 2.5
void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}




