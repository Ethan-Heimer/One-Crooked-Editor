#include "texteditor.hpp"
#include "bufferfilehandler.h"

#include "configevents.hpp"
#include "rendereditorcommand.hpp"
#include "rendererevents.hpp"
#include "runtimeevents.hpp"

#include "editorcontext.h"
#include "editorstateparameters.h"
#include "editorstates.h"
#include "editorvisualstate.hpp"
#include "inputhandler.hpp"
#include "isystem.hpp"

#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

#include "inputconstants.hpp"
#include "terminalenviormentevents.hpp"

#include <cstdlib>
#include <memory>
#include <queue>
#include <sstream>
#include <string>

using namespace CrookedEditor::Systems;
using namespace CrookedEditor::Buffers;
using namespace CrookedEditor::States;

using namespace Editor;
using namespace Editor::States;

using namespace CrookedEditor::Components;
using namespace Rendering::Commands;

struct TextEditor::Impl{
    API::SystemID SID;
    Events::RequestBus* requestBus;

    std::string fileName;
    std::unique_ptr<EditorContext> context;

    std::queue<int> inputQueue;
    std::stringstream inputStream;

    TextEditorVisualState visualState{};

    Impl(API::SystemID& SID, Events::RequestBus* requestBus)
        : SID(SID), requestBus(requestBus){}

    API::SystemStartOptions Start(){
        auto request = requestBus->SendRequest<API::ConfigGetValue>("fileName");
        if(request.has_value()){
            fileName = std::get<const char*>(request.value()); 
        } else {
            fileName = "newfile.txt";
        }

        context = std::make_unique<EditorContext>(BufferFileInterpreter{}, 
            DefaultStates<NormalState, InsertState>{}, fileName);

        requestBus->ListenForEvent<InputCaptured>([&](int input){
            inputQueue.push(input);
        });

        return API::SystemStartOptions{.SeperateThread = true};
    }

    bool Update(){
        int key;
        while(!inputQueue.empty()){
            key = inputQueue.front();
            inputQueue.pop();

            if(key != API::NoInput)
                inputStream << static_cast<char>(key);
        }

        context->Update(std::move(inputStream.str()));
        inputStream.str("");

        if(context->quit){
            requestBus->SendRequest<API::RuntimeQuit>();
        }

        auto response = requestBus->SendRequest<API::TermGetSize>();
        std::pair<int, int> size;
        if(response.has_value())
            size = response.value();
 
        Renderer::RenderEditorCommand renderEditorCommand{*context->buffer, visualState, size.first, size.second};
        requestBus->SendRequest<API::PublishRenderCommand>(std::move(renderEditorCommand));

        return false;
    }

    void Stop(API::SystemTerminateReasons reasons){}
};

TextEditor::TextEditor() : API::ISystem(){};
TextEditor::TextEditor(API::SystemID SID, Events::RequestBus* requestBus) 
    : ISystem(SID, requestBus), pImpl(std::make_unique<Impl>(SID, requestBus)){
}

TextEditor::~TextEditor() = default;

CrookedEditor::API::SystemStartOptions TextEditor::Start(){
    return pImpl->Start();
}

bool TextEditor::Update(){
    return pImpl->Update();
}

void TextEditor::Stop(CrookedEditor::API::SystemTerminateReasons reasons){
    pImpl->Stop(reasons);
}

std::shared_ptr<CrookedEditor::API::ISystem> TextEditor::Instanciate(CrookedEditor::API::SystemID SID, 
        Events::RequestBus* requestBus) const{
    return std::make_shared<TextEditor>(SID, requestBus);
}
