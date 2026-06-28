#pragma once

#include "bufferfilehandler.h"
#include "bufferfilehandlerfactory.h"
#include "editorstatecontext.h"
#include "editorcontext.h"
#include "editorcontextfactory.h"
#include "editorstatecontextfactory.h"

using namespace std;
using namespace Editor;
using namespace Editor::States;

namespace Editor::Metaprogramming{
    template<typename... T>
    constexpr auto DefaultEditorFactory(){ 
            return EditorContextFactory<EditorContext, 
            StateContextFactory<StateContext, T...>, 
            BufferFileHandlerFactory<BufferFileHandler>>{};
    }
}
