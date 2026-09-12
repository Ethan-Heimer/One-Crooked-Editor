#include "texteditor.hpp"
#include "application.hpp"
#include "bufferfilehandler.h"
#include "configvault.hpp"

#include "editorcontext.h"
#include "editorrenderingstate.hpp"
#include "editorstateparameters.h"
#include "editorstates.h"
#include <memory>

using namespace CrookedEditor::Application;
using namespace CrookedEditor::Buffers;
using namespace CrookedEditor::States;

using namespace Editor;
using namespace Editor::States;

using namespace CrookedEditor::Renderer;

struct TextEditor::Impl{
    Application& app;
    ConfigVault& config;
    ThreadHandle handle = -1;

    Impl(Application& app, ConfigVault& config) : app(app), config(config){}

    ~Impl(){
    
    }
    
    void Start(std::string_view fileName, std::function<int()> getNextKey, 
            std::function<void(const IEditable& buffer, EditorRenderingState& renderingState)> renderEditor){
        if(handle >= 0){
            Stop();
        }

        auto editorJob = [this, fileName, getNextKey, renderEditor](const std::atomic<bool>& quitToken){
            EditorContext context{BufferFileInterpreter{}, DefaultStates<NormalState, InsertState>{}, fileName.data()};
            stringstream inputStream;

            EditorRenderingState renderingState{};
            while(!quitToken){
                int key;
                while((key = getNextKey()) != -1){
                    inputStream << static_cast<char>(key);
                }

                context.Update(std::move(inputStream).str());
                inputStream.str("");
                inputStream.clear();

                if(context.quit)
                    app.Quit();

                renderEditor(*context.buffer, renderingState);
                std::this_thread::sleep_for(1ms);
            };

        }; 

        handle = app.SpawnThread(editorJob);
    }
    
    void Stop(){
        if(handle < 0)
            return; 

        app.KillThread(handle);
        handle = -1; 
    }
};


TextEditor::TextEditor(Application& app, ConfigVault& config) : pImpl(std::make_unique<Impl>(app, config)){}

TextEditor::~TextEditor(){
    Stop();
}

void TextEditor::Start(std::string_view fileName, std::function<int()> getNextKey, 
        std::function<void(const IEditable& buffer, EditorRenderingState& renderingState)> renderEditor){
    pImpl->Start(fileName, getNextKey, renderEditor);
}

void TextEditor::Stop(){
    pImpl->Stop();
}
