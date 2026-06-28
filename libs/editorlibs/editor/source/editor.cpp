#include "editor.h"
#include <memory>

using namespace Editor;

void EditorContext::Initialize( 
                    shared_ptr<Buffers::IBufferFileHandlerFactory> bufferFileHandlerFactory,
                    shared_ptr<States::IStateContextFactory> stateContextFactory, queue<int>* inputQueue, string fileName){
    this->bufferFileHandler = bufferFileHandlerFactory->Instanciate(fileName);
    this->buffer = bufferFileHandler->LoadFromFile();

    this->stateContext = stateContextFactory->Instanciate(this->bufferFileHandler, buffer, inputQueue, &quit);
}

void EditorContext::Update(){
    stateContext->Update();
}
