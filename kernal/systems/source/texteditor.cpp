#include "texteditor.hpp"
#include "bufferfilehandler.h"

#include "configconstants.hpp"
#include "configevents.hpp"
#include "rendererevents.hpp"
#include "resultbatch.hpp"
#include "runtimeevents.hpp"
#include "inputevents.hpp"

#include "rendereditorcommand.hpp"
#include "ringbuffer.hpp"

#include "editorcontext.h"
#include "editorstateparameters.h"
#include "editorstates.h"
#include "editorvisualstate.hpp"
#include "isystem.hpp"

#include "systemstartoptions.hpp"
#include "systemterminatereasons.hpp"

#include "inputconstants.hpp"
#include "terminalenviormentevents.hpp"

#include <cstdlib>
#include <memory>
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
    TextEditorVisualState visualState{};
    Events::RequestBus* requestBus;

    std::string fileName;
    std::unique_ptr<EditorContext> context;

    Queues::RingBuffer<int, 256> inputBuffer;

    std::stringstream inputStream;
    std::function<void(int)> inputListener = [&](int input){ 
        inputBuffer.Push(input);
    };

    Events::ResultBatch batch;
    Impl(API::SystemID& SID, Events::RequestBus* requestBus)
        : requestBus(requestBus){}

    API::SystemStartOptions Start(){
        auto request = requestBus->SendRequest<API::ConfigGetValue>(API::FileArgName);
        if(request.has_value()){
            fileName = std::get<std::string>(request.value()); 
        } else {
            fileName = "newfile.txt";
        }

        context = std::make_unique<EditorContext>(BufferFileInterpreter{}, 
            DefaultStates<NormalState, InsertState>{}, fileName);

        batch = requestBus->ListenForEvent<API::InputCaptured>(inputListener);
        batch.Log<API::RuntimeLog>(*requestBus);

        return API::SystemStartOptions{.SeperateThread = true};
    }

    bool Update(){
        int key;
        while(inputBuffer.Consume(key)){
            if(key != API::NoInput)
                inputStream << static_cast<char>(key);
        }

        context->Update(std::move(inputStream.str()));
        inputStream.str("");

        if(context->quit){
            batch = requestBus->SendRequest<API::RuntimeQuit>();
        }

        auto response = requestBus->SendRequest<API::TermGetSize>();
        std::pair<int, int> size;
        if(response.has_value())
            size = response.value();
 
        Renderer::RenderEditorCommand renderEditorCommand{*context->buffer, visualState, size.first, size.second};
        batch = requestBus->SendRequest<API::PublishRenderCommand>(std::move(renderEditorCommand));
        batch.Log<API::RuntimeLog>(*requestBus);

        return false;
    }

    void Stop(API::SystemTerminateReasons reasons){
        batch = requestBus->RemoveEventListener<API::InputCaptured>(inputListener);
        batch.Log<API::RuntimeLog>(*requestBus);
    }
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
