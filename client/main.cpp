#include <format>
#include <memory>
#include <fcntl.h>
#include <poll.h>

#include <chrono>

#include <sstream>
#include <sys/poll.h>
#include <unistd.h>
#include <termios.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include "bufferfilehandler.h"
#include "editorcontext.h"
#include "editorstates.h"
#include "iinputmanager.h"
#include "rendercommand.hpp"
#include "renderstringcommand.hpp"
#include "tuirenderer.hpp"

#include "inputmanager.h"
#include "safequeue.h"

using namespace std;
using namespace std::chrono;
using namespace Editor;

using namespace Systems::Input;
using namespace Editor::States;

using namespace CrookedEditor::Buffers;
using namespace CrookedEditor::States;

using namespace Rendering;
using namespace Rendering::Commands;

void process_mem_usage(double& vm_usage, double& resident_set);
void EditorLoop(bool* quitToken, shared_ptr<SafeQueue<unique_ptr<RenderCommand>>> renderQueue, shared_ptr<SafeQueue<int>> inputQueue, std::string fileName){
    EditorContext context{BufferFileInterpreter{}, DefaultStates<NormalState, InsertState>{}, fileName.data()};
    stringstream inputStream;

    while(!*quitToken){

        while(!inputQueue->empty()){
            inputStream << static_cast<char>(inputQueue->front());
            inputQueue->pop();
        }

        context.Update(std::move(inputStream).str());
        inputStream.str("");
        inputStream.clear();

        *quitToken = context.quit;


        auto start = context.buffer->BeginStepsFromCurrentLine(-5);
        auto end = context.buffer->EndStepsFromCurrentLine(15-1);

        int i = 0;
        for(auto line = start ; line != end; ++line){
            std::string str = std::format("{:3}| {}", line.LineNumber(), (*line)); 
            unique_ptr<RenderCommand> command = std::make_unique<RenderString>(0, i, str);
            renderQueue->move(std::move(command));
            i++;
        }
    }

}

void IOLoop(bool* quitToken, shared_ptr<SafeQueue<unique_ptr<RenderCommand>>> renderQueue, shared_ptr<SafeQueue<int>> inputQueue){
    Rendering::TUIRenderer renderer{};
    milliseconds msd {};
    double vm, rss;
    shared_ptr<IInputManager> inputManager = std::make_shared<InputManager>();

    while(!*quitToken){
        milliseconds msb = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        process_mem_usage(vm, rss);

        int ch = inputManager->GetKeyInput();

        if(ch != 0){
            if(ch == 8)
                inputQueue->push('\b');
            else
                inputQueue->push(ch);
        }

        while(!renderQueue->empty()){
            std::unique_ptr<RenderCommand> command = std::move(renderQueue->front());
            renderer.DoCommand(*command);
            renderQueue->pop(); 

        }
        renderer.Display(msd.count());

        milliseconds msa = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        msd = msa-msb;
    }

}

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    bool quitToken = false;

    shared_ptr<SafeQueue<unique_ptr<RenderCommand>>> renderQueue = std::make_shared<SafeQueue<unique_ptr<RenderCommand>>>();
    shared_ptr<SafeQueue<int>> inputQueue = std::make_shared<SafeQueue<int>>();

    string fileName{argv[1]};
 
    std::thread editor{EditorLoop, &quitToken, renderQueue, inputQueue, fileName};
    std::thread IO{IOLoop, &quitToken, renderQueue, inputQueue};

    editor.join();
    IO.join();
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




