#include "editorcontext.h"

using namespace Editor;

EditorContext::EditorContext( 
    FileHandling::FileHandler&& fileHandler, States::StateParameters stateParams, std::string fileName) 
        : fileHandler(std::forward<FileHandling::FileHandler>(fileHandler)){

    this->fileHandler.fileName = fileName;
    this->buffer = this->fileHandler.LoadFromFile();

    //this is my suspition
    auto saveBuffer = [this](){
        this->fileHandler.SaveToFile(*buffer);
    };

    this->undoHandler = std::make_unique<Mutators::UndoHandler>();
    this->mutationManager = std::make_unique<Mutators::MutatorManager>(*buffer, *undoHandler);
    this->stateContext = std::make_unique<States::StateContext>(std::move(stateParams), *buffer, 
            *mutationManager, saveBuffer, *undoHandler, &quit);
}

EditorContext::EditorContext(EditorContext&& other){

    fileHandler = std::move(other.fileHandler);

    undoHandler = std::move(other.undoHandler);
    mutationManager = std::move(other.mutationManager);

    stateContext = std::move(other.stateContext);

    buffer = std::move(other.buffer);
    
}

EditorContext& EditorContext::operator=(EditorContext&& other){
    fileHandler = std::move(other.fileHandler);

    undoHandler = std::move(other.undoHandler);
    mutationManager = std::move(other.mutationManager);
    stateContext = std::move(other.stateContext);

    buffer = std::move(other.buffer);
               
    return *this;
}

void EditorContext::Update(std::string_view input){
    this->stateContext->Update(input);
}

std::string EditorContext::CurrentModeName(){
    return this->stateContext->CurrentStateName();
}
