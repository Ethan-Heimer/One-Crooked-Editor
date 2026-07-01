#include "editorcommands/editorcommandmanager.h"
#include "editorcommands/ieditorcommand.h"
#include "ieditable.h"
#include "ieditorcommandmanager.h"
#include <memory>
#include <stack>

using namespace Editor::Commands;
using Command =std::shared_ptr<IEditorCommand>;

struct EditorCommandManager::Impl{
    /*
     * Todo: Make undo Tree one day
     */

    std::stack<Command> undoStack;
    std::stack<Command> redoStack;

    void Undo(){
        if(undoStack.empty())
            return;

        std::shared_ptr<IEditorCommand> command = undoStack.top();
        undoStack.pop();

        command->Undo();
        redoStack.push(std::move(command));
    }

    void Redo(){
        if(redoStack.empty())
            return;

        std::shared_ptr<IEditorCommand> command = redoStack.top();
        redoStack.pop();

        command->Execute();
        undoStack.push(std::move(command)); 
    }

    void ExecuteCommand(std::shared_ptr<IEditorCommand> command){
        command->Execute();
        undoStack.push(std::move(command));
        redoStack = {};
    }
};

EditorCommandManager::EditorCommandManager(weak_ptr<IEditable> buffer) : IEditorCommandManager(buffer),
                                pImpl(std::make_unique<Impl>()){};

EditorCommandManager::~EditorCommandManager() = default;

void EditorCommandManager::Undo(){
    pImpl->Undo();
}

void EditorCommandManager::Redo(){
    pImpl->Redo();
}

void EditorCommandManager::ExecuteCommand(std::shared_ptr<IEditorCommand> command){
    pImpl->ExecuteCommand(std::move(command));
}
