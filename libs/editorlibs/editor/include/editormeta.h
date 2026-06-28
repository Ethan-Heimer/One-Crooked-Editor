#pragma once

#include <memory>

#include "bufferfilehandler.h"
#include "bufferfilehandlerfactory.h"
#include "editorstatecontext.h"
#include "editor.h"
#include "editorfactory.h"
#include "insertstate.h"
#include "ieditorfactory.h"

#include "normalstate.h"
#include "editorstatecontextfactory.h"

using namespace std;
using namespace Editor;
using namespace Editor::States;

namespace Editor::Metaprogramming{
    template<typename... T>
    constexpr auto CreateDefaultEditorFactory(){ 
            return EditorContextFactory<EditorContext, 
            StateContextFactory<StateContext, T...>, 
            BufferFileHandlerFactory<BufferFileHandler>>{};
    }
}
