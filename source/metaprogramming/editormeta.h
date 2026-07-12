#pragma once

#include "bufferfilehandler.h"
#include "bufferfilehandlerfactory.h"
#include "editorcommandmanager.h"
#include "editorstatecontext.h"
#include "editorcontext.h"
#include "editorcontextfactory.h"
#include "editorstatecontextfactory.h"
#include "editorcommandmanagerfactory.h"
#include "editorundohandler.h"
#include "editorundohandlerfactory.h"
#include "ieditorundohanderfactory.h"
#include <memory>

using namespace std;
using namespace Editor;
using namespace Editor::States;
using namespace Buffers;

namespace Editor::Metaprogramming{
    template<typename E, typename... T>
    constexpr auto DefaultEditorFactory(){ 
                shared_ptr<States::IStateContextFactory> stateContextFactory
                    = make_shared<StateContextFactory<StateContext, T...>>();
                
                shared_ptr<IEditorFileHandlerFactory<E>> bufferFileHandlerFactory
                    = make_shared<BufferFileHandlerFactory<E, BufferFileHandler>>();

                shared_ptr<IEditorCommandManagerFactory> commandManagerFactory
                    = make_shared<EditorCommandManagerFactory<EditorCommandManager>>();

                shared_ptr<IEditorUndoHandlerFactory> commandUndoHandlerFactory =
                    make_shared<EditorUndoHandlerFactory<EditorUndoHandler>>();

            return EditorContextFactory<E, EditorContext<E>>(std::move(stateContextFactory),
                    std::move(bufferFileHandlerFactory), std::move(commandManagerFactory), std::move(commandUndoHandlerFactory));
    }
}
