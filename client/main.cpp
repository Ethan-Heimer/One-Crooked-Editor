#include <cstdio>
#include <format>
#include <functional>
#include <future>
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
#include <unistd.h>
#include <vector>

#include "bufferfilehandler.h"
#include "editorcontext.h"
#include "editorstates.h"
#include "ieditable.h"
#include "lspclient.hpp"
#include "rendercommandqueue.hpp"
#include "responses/lspinitilizeresponse.hpp"
#include "terminal.hpp"
#include "tuirenderer.hpp"
#include "standardrenderercommands.hpp"
#include "rendering/include/rendereditorcommand.hpp"

#include "requests/lspinitializerequest.hpp"
#include "requests/lspsemantictokensfullrequest.hpp"

#include "notifications/lspinitializednotification.hpp"
#include "notifications/lspdocumentdidopennotification.hpp"

#include "inputmanager.h"
#include "safequeue.h"

#ifdef __APPLE__
    #include <mach/mach.h>
#endif

using namespace std;
using namespace std::chrono;
using namespace std::filesystem;

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
std::string GetLSPResponse(long fd);

void EditorLoop(bool* quitToken, std::function<void(const IEditable&, EditorRenderingState& renderingState)> renderEditor, 
         shared_ptr<SafeQueue<int>> inputQueue, std::shared_ptr<TerminalController> terminalController, std::string fileName){

    LSP::LSPClient lspClient{};
    lspClient.StartLSP("clangd", "--log=verbose --background-index");

    EditorContext context{BufferFileInterpreter{}, DefaultStates<NormalState, InsertState>{}, fileName.data()};
    stringstream inputStream;

    EditorRenderingState renderingState{};

    // LSP INIT -- handled in start lsp?
    auto responseFuture = lspClient.SendRequest(LSP::InitializeRequest{});
    auto response = responseFuture.Get();
    bool test = std::get<bool>(response.capabilities["compilationDatabase.automaticReload"]);

    lspClient.SendNotification(LSP::InitializedNotification{});
    lspClient.SendNotification(LSP::DocumentDidOpenNotification{fileName});
    // END

    auto semanticResponseFuture = lspClient.SendRequest(LSP::SemanticTokensFullRequest{fileName});
    auto semanticResponse = semanticResponseFuture.Get();

    for(auto i : semanticResponse.data){
        std::cout << i << " ";
    }

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
        std::this_thread::sleep_for(1ms);
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
        std::this_thread::sleep_for(1ms);
    }
}

int main(int argc, char** argv){
    if(argc < 2){
        return 1; 
    }

    int* p = new int;

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

    //this_thread::sleep_for(std::chrono::milliseconds(100000));
}


void process_mem_usage(double& vm_usage, double& resident_set)
{
   // Source - https://stackoverflow.com/a/671389
   // Posted by Don Wakefield, modified by community. See post 'Timeline' for change history
   // Retrieved 2026-08-05, License - CC BY-SA 2.5
   vm_usage = 0;
   resident_set = 0;

#ifdef __linux__
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
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
#endif

#ifdef __APPLE__
    task_vm_info_data_t vm_info;
    mach_msg_type_number_t count = TASK_VM_INFO_COUNT;

    kern_return_t kr = task_info(
        mach_task_self(),
        TASK_VM_INFO,
        (task_info_t)&vm_info, 
        &count
    );

    if(kr == KERN_SUCCESS){
       vm_usage  = vm_info.phys_footprint;
    }
#endif
}

std::string GetLSPResponse(long fd){
    //grab header
    std::stringstream buffer;
    bool readingHeader = true;
    while(readingHeader){
        char ch;
        int n = read(fd, &ch, 1);
        if(n > 0){
            //marks end of header transmition
            if(ch == '\r'){
                //consume following '\n\r\n'
                read(fd, &ch, 1); 
                read(fd, &ch, 1); 
                read(fd, &ch, 1); 

                readingHeader = false;
                }else {
                    buffer << ch;
                }
            }
        }

        //get content length
        int contentLength = 0;
        sscanf(buffer.str().c_str(), "Content-Length: %d", &contentLength);

        //get the rest of the content
        std::vector<char> contentBuffer;
        contentBuffer.resize(contentLength);

        int bytesRead = 0;
        while(bytesRead < contentLength){
           int n = read(fd, &contentBuffer[bytesRead], contentLength - bytesRead);
           bytesRead += n;
        }

        return std::string{contentBuffer.begin(), contentBuffer.end()};
}

