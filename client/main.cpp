#include <cstdio>
#include <cstring>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <fcntl.h>
#include <poll.h>

#include <chrono>

#include <sstream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <thread>

#include "bufferfilehandler.h"
#include "editorcontext.h"
#include "editorstates.h"
#include "ieditable.h"
#include "rendercommandqueue.hpp"
#include "terminal.hpp"
#include "tuirenderer.hpp"
#include "standardrenderercommands.hpp"
#include "rendering/include/rendereditorcommand.hpp"

#include "inputmanager.h"
#include "safequeue.h"

using namespace std;
using namespace std::chrono;
using namespace Editor;

using namespace Systems::Input;
using namespace Editor::States;

using namespace CrookedEditor::Buffers;
using namespace CrookedEditor::States;
using namespace CrookedEditor::Renderer;

using namespace Rendering;
using namespace Rendering::Commands;

using namespace Terminal;

void process_mem_usage(double& vm_usage, double& resident_set);
pid_t StartLSP(std::array<int, 2>& parentToChildPipe, std::array<int, 2>& childToParentPipe){
    if(pipe(parentToChildPipe.data()) < 0 || pipe(childToParentPipe.data()) < 0){
        std::cerr << "Pipe Creation failed to LSP" << std::endl;
        return -1;
    }

    pid_t pid = fork();
    if(pid < 0){
        std::cerr << "error spawining child process" << std::endl;
        return -1;
    }

    if(pid == 0){
        // in child process
        close(parentToChildPipe[1]); // close write end to child input pipe
        close(childToParentPipe[0]); // close read end to child output pipe
        
        dup2(parentToChildPipe[0], STDIN_FILENO);  // Redirect stdin to be the input pipe
        dup2(childToParentPipe[1], STDOUT_FILENO); // Redirect stdout to be the output pipe
        dup2(STDOUT_FILENO, STDERR_FILENO);
                                                   //
        execlp("clangd", "clangd", "--log=verbose", "--background-index", NULL);
        exit(0);
    }
    else{

        close(parentToChildPipe[0]); // close read end to parent output pipe
        close(childToParentPipe[1]); // close write end to parent input pipe
        
        int flags = fcntl(childToParentPipe[0], F_GETFL, 0);
        fcntl(childToParentPipe[0], F_SETFL, flags | O_NONBLOCK);
    }

    return 1;
}

void EditorLoop(bool* quitToken, std::function<void(const IEditable&, EditorRenderingState& renderingState)> renderEditor, 
         shared_ptr<SafeQueue<int>> inputQueue, std::shared_ptr<TerminalController> terminalController, std::string fileName){

    std::array<int, 2> parentToChildPipe;
    std::array<int, 2> childToParentPipe;
    pid_t processID = StartLSP(parentToChildPipe, childToParentPipe);  
    if(processID != 0){

        EditorContext context{BufferFileInterpreter{}, DefaultStates<NormalState, InsertState>{}, fileName.data()};
        stringstream inputStream;

        EditorRenderingState renderingState{};

        std::string request = 
            "{"
            "  \"jsonrpc\": \"2.0\","
            "  \"id\": 1,"
            "  \"method\": \"initialize\","
            "  \"params\": {"
            "    \"processId\": " + std::to_string(getpid()) + ","
            "    \"rootUri\": null,"
            "    \"capabilities\": {}"
            "  }"
            "}";
        std::string message = "Content-Length: " + std::to_string(request.length()) + "\r\n\r\n" + request;
        write(parentToChildPipe[1], message.c_str() , message.length());

        while(!*quitToken){
            while(!inputQueue->empty()){
                inputStream << static_cast<char>(inputQueue->front());
                inputQueue->pop();
            }

            context.Update(std::move(inputStream).str());
            inputStream.str("");
            inputStream.clear();

            *quitToken = context.quit;
            renderEditor(*context.buffer, renderingState);

            // make non blocking?
            char ch;
            int n = read(childToParentPipe[0], &ch, 1);
            if(n > 0){
                std::fstream ostream{"debug.txt", std::ios::in | std::ios::out | std::ios::app};
                ostream << ch;
            }
        }
    }
}

void IOLoop(bool* quitToken, shared_ptr<RenderingCommandQueue> renderQueue, 
        shared_ptr<SafeQueue<int>> inputQueue, shared_ptr<TerminalController> terminalController){
    Rendering::TUIRenderer renderer{*terminalController};
    InputManager inputManager{*terminalController};

    milliseconds msd {};
    double vm, rss;

    while(!*quitToken){
        milliseconds msb = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        process_mem_usage(vm, rss);

        int ch = inputManager.GetKeyInput();
        if(ch != 0){
            inputQueue->push(ch);
        }

        int row, col{};
        terminalController->GetTerminalSize(row, col);

        if(!renderQueue->Empty()){
            renderQueue->NewCommand<RenderString>(row-1, 0, std::format("Diagnositcs | MS:{} MEM: {} Mb RSS: {}", msd.count(), vm/1024, rss));
            renderer.DoCommands(*renderQueue);
            renderer.Display();
        }

        milliseconds msa = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        msd = msa-msb;
    }
}

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    bool quitToken = false;
    shared_ptr<SafeQueue<int>> inputQueue = std::make_shared<SafeQueue<int>>();

    shared_ptr<TerminalController> terminalController = std::make_shared<TerminalController>();
    shared_ptr<RenderingCommandQueue> renderQueue = std::make_shared<RenderingCommandQueue>(*terminalController);

    string fileName{argv[1]};

    auto renderEditor = [renderQueue](const IEditable& buffer, EditorRenderingState& renderingState){
        renderQueue->NewCommand<CrookedEditor::Renderer::RenderEditorCommand>(std::ref(buffer), std::ref(renderingState));
    };
 
    std::thread editor{EditorLoop, &quitToken, renderEditor, inputQueue, terminalController, fileName};
    std::thread IO{IOLoop, &quitToken, renderQueue, inputQueue, terminalController};

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




