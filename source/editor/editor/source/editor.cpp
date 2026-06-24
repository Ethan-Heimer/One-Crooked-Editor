#include "editor.h"
#include "context.h"
#include "icontext.h"
#include "ieditor.h"
#include "iinputmanager.h"
#include <memory>
#include <fstream>


Editor::Editor::Editor(EditorPasskey passkey, 
        weak_ptr<Input::IInputManager> inputManager, string fileName, 
        weak_ptr<States::StateContextFactory> stateContextFactory) : IEditor(passkey){
    this->inputManager = inputManager;
    this->fileName = fileName;

    this->stateContextFactory = stateContextFactory;
}

void Editor::Editor::Initialize(){
    this->buffer = std::make_shared<Buffer>(fileName);

    this->stateContext = stateContextFactory.lock()->Instanciate<States::StateContext>(GetWeakPtr(), inputManager);
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
