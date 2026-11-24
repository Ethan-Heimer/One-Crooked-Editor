#include "texteditor.hpp"
#include "bufferfilehandler.h"

#include "configevents.hpp"
#include "runtimeevents.hpp"

#include "editorcontext.h"
#include "editorstateparameters.h"
#include "editorstates.h"
#include "editorvisualstate.hpp"
#include "inputhandler.hpp"
#include "isystem.hpp"
#include "rendercommandconstructor.hpp"
#include "rendereditorcommand.hpp"
#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

#include "inputconstants.hpp"
#include "editorvisualstatehelper.hpp"
#include "terminalrenderer.hpp"

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
    const int lineColumnSpace = 4;
    const int verticalScrollThreshold = 5;
    const int invisableRows = 2;

    API::SystemID SID;
    Events::RequestBus& requestBus;

    std::string fileName;
    std::unique_ptr<EditorContext> context;

    std::queue<int> inputQueue;

    std::stringstream inputStream;

    TextEditorVisualState visualState{};
    EditorVisualStateHelper visualStateHelper{}; 

    Impl(API::SystemID& SID, Events::RequestBus& requestBus)
        : SID(SID), requestBus(requestBus){}

    API::SystemStartOptions Start(){
        auto request = requestBus.SendRequest<API::ConfigGetValue>("fileName");
        if(request.has_value()){
            fileName = std::get<const char*>(request.value()); 
        } else {
            fileName = "newfile.txt";
        }

        context = std::make_unique<EditorContext>(BufferFileInterpreter{}, 
            DefaultStates<NormalState, InsertState>{}, fileName);

        requestBus.ListenForEvent<InputCaptured>([&](int input){
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
        inputStream.clear();
        inputStream.str("");

        if(context->quit){
            requestBus.SendRequest<API::RuntimeQuit>();
        }

        visualStateHelper.UpdateVisualState(requestBus, 
                        *context, visualState, invisableRows, verticalScrollThreshold, 
                        lineColumnSpace);
        
        auto temp = visualState.Dispatch();
        auto visualStatePtr = std::make_shared<const TextEditorVisualState>(std::move(temp));

        requestBus.SendRequest<PublishRenderCommand>(
                CreateCommandConstructor<Renderer::RenderEditorCommand>(visualStatePtr));

        return false;
    }

    void Stop(API::SystemTerminateReasons reasons){}
};

TextEditor::TextEditor(API::SystemID SID, Events::RequestBus& requestBus) 
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
