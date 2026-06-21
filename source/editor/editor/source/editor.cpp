#include "editor.h"
#include "context.h"
#include "ieditor.h"
#include "iinputmanager.h"
#include <memory>
#include <fstream>

void Editor::Editor::Initialize(weak_ptr<Input::IInputManager> inputManager, const string fileName){
    this->inputManager = inputManager;
    this->fileName = fileName;

    this->buffer = std::make_shared<Buffer>(fileName);

    this->stateContext = std::make_shared<States::StateContext>();
    this->stateContext->Initialize(GetWeakPtr(), inputManager);
}

void Editor::Editor::Update(){
    stateContext->Update();
}

void Editor::Editor::Save(){
    ofstream saveFile{fileName};
    if(saveFile.is_open()){
        for(auto node : buffer->buffer){
            saveFile << ((string)*node).c_str() << endl;
        }

        saveFile.close();
    }
}

std::weak_ptr<Editor::IEditor> Editor::Editor::GetWeakPtr(){
    return weak_from_this();
}
