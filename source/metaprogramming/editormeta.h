#pragma once

#include "bufferfilehandler.h"
#include "bufferfilehandlerfactory.h"
#include "editorcommandmanager.h"
#include "editorstatecontext.h"
#include "editorcontext.h"
#include "editorcontextfactory.h"
#include "editorstatecontextfactory.h"
#include "editorcommandmanagerfactory.h"

using namespace std;
using namespace Editor;
using namespace Editor::States;
using namespace Buffers;

namespace Editor::Metaprogramming{
    template<typename E, typename... T>
    constexpr auto DefaultEditorFactory(){ 
            return EditorContextFactory<E, EditorContext<E>, 
            StateContextFactory<StateContext, T...>, 
            BufferFileHandlerFactory<E, BufferFileHandler>,
            EditorCommandManagerFactory<EditorCommandManager>>{};
    }
}
