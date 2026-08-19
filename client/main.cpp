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

#include "inputmanager.h"
#include "safequeue.h"

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
    //have lsp client handle this and init it in start lsp?
    std::thread lspThread = std::thread([](LSP::LSPClient& lspClient, bool* quitToken){
        lspClient.StartLSP("clangd", "--log=verbose --background-index");

        while(!*quitToken){
            lspClient.PollReponses();
            std::this_thread::sleep_for(1ms);
        }
    }, std::ref(lspClient), quitToken);

    EditorContext context{BufferFileInterpreter{}, DefaultStates<NormalState, InsertState>{}, fileName.data()};
    stringstream inputStream;

    EditorRenderingState renderingState{};

    auto responseFuture = lspClient.SendRequestAsync<LSP::InitializeResponse>(LSP::InitializeRequest{});
    auto response = responseFuture.get();
    bool test = std::get<bool>(response.capabilities["compilationDatabase.automaticReload"]);

    //lsp client communication spec:
    // client.SendRequest(ILSPRequest);
    // client.SendNotification(ILSPNoticication);
    //
    // -- client needs to defines events on when the SERVER sends a notifications
    // 

    // Start Initilization handshake
    /*
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
    write(lspClient.parentToChildPipe[1], message.c_str() , message.length());

    std::fstream ostream{"debug.txt", std::ios::out};
    std::string response = GetLSPResponse(lspClient.childToParentPipe[0]);
    ostream << response;

    request = 
            "{"
            "  \"jsonrpc\": \"2.0\","
            "  \"method\": \"initialized\","
            "  \"params\": {}"
            "}";

    message = "Content-Length: " + std::to_string(request.length()) + "\r\n\r\n" + request;
    write(lspClient.parentToChildPipe[1], message.c_str() , message.length());
    // End initilization handshake
        
    // textdocument/didOpen notification
    path absolutePath = canonical(fileName);
    std::ifstream fileContentStream{fileName};
    //make sure file opened
    std::stringstream fileContentBuffer; 

    char ch;
    while(fileContentStream.get(ch)){
        switch(ch){
            case '\n':
                fileContentBuffer << "\\n";
                break;

            case '\r':
                fileContentBuffer << "\\r";
                break;

            case '"':
                fileContentBuffer << "\\\"";
                break;

            default:
                fileContentBuffer << ch;
                break;
        }
    }
       
    // document did open notification
    // * the server needs to keep track of the document in memory
    request = 
            "{"
            "  \"jsonrpc\": \"2.0\","
            "  \"method\": \"textDocument/didOpen\","
            "  \"params\": {"
            "       \"textDocument\": {"
            "           \"uri\": \"file:///" + absolutePath.string() + "\","
            "          \"languageId\": \"scss\","
            "          \"version\": 1,"
            "          \"text\": \"" + fileContentBuffer.str() + "\""
            "       }"
            "   }"
            "}";

    message = "Content-Length: " + std::to_string(request.length()) + "\r\n\r\n" + request;
    write(lspClient.parentToChildPipe[1], message.c_str() , message.length());
    // end document did open notification

    // a notification is sent back with diagnostic data about the lsp's config?
    response = GetLSPResponse(lspClient.childToParentPipe[0]);
    ostream << std::endl;
    ostream << response;

    //another notification is sent back with diagnostic data about the document
    response = GetLSPResponse(lspClient.childToParentPipe[0]);
    ostream << std::endl;
    ostream << response;

    request = "{"
    "\"jsonrpc\": \"2.0\","
    "\"id\": 2,"
    "\"method\": \"textDocument/semanticTokens/full\"," //there is a range version too, probably for when edits are made
    "\"params\": {"
    "\"textDocument\": {"
    "\"uri\": \"file:///" + absolutePath.string() + "\""
    "}"
    "}"
    "}";

    message = "Content-Length: " + std::to_string(request.length()) + "\r\n\r\n" + request;
    write(lspClient.parentToChildPipe[1], message.c_str() , message.length());

    // response back with hightlighting
    response = GetLSPResponse(lspClient.childToParentPipe[0]);
    ostream << std::endl;
    ostream << response;
    */

    // syntax tokens are grouped into sets of 5
    // clangd doesnt highligt things like primitives, operators, comments, etc
    // a primitive highlighter should be developed to allow for these

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

    lspThread.join();
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

