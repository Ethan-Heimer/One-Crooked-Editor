#include "editorcontext.h"

using namespace Editor;

EditorContext::EditorContext( 
    FileHandling::FileHandler fileHandler, States::StateParameters stateParams,
    std::queue<int>* inputQueue, std::string fileName) : fileHandler(std::move(fileHandler)){

    this->fileHandler.fileName = fileName;
    this->buffer = this->fileHandler.LoadFromFile();

    this->undoHandler = std::make_unique<Mutators::UndoHandler>();
    this->mutationManager = std::make_unique<Mutators::MutatorManager>(*buffer, *undoHandler);
    this->stateContext = std::make_unique<States::StateContext>(std::move(stateParams), *buffer, 
            *mutationManager, [this](){this->fileHandler.SaveToFile(*buffer);}, *undoHandler, inputQueue, &quit);
}

void EditorContext::Update(){
    this->stateContext->Update();
}

std::string EditorContext::CurrentModeName(){
    return this->stateContext->CurrentStateName();
}
