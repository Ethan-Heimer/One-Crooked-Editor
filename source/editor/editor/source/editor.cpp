#include "editor.h"
#include "iinputmanager.h"
#include <memory>

void Editor::Editor::Initialize(weak_ptr<Systems::Input::IInputManager> inputManager, 
                    string fileName, weak_ptr<Buffers::IBufferFileHandlerFactory> bufferFileHandlerFactory,
                    weak_ptr<States::IStateContextFactory> stateContextFactory){
    this->bufferFileHandler = bufferFileHandlerFactory.lock()->Instanciate(fileName);
    this->buffer = bufferFileHandler->LoadNewBuffer();

    this->stateContext = stateContextFactory.lock()->Instanciate(this->bufferFileHandler, buffer, inputManager, &quit);
}

void Editor::Editor::Update(){
    stateContext->Update();
}

std::weak_ptr<Editor::IEditor> Editor::Editor::GetWeakPtr(){
    return weak_from_this();
}
