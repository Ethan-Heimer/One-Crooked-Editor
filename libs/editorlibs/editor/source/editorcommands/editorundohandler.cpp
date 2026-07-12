#include "editorundohandler.h"

#include "editorcommands/ieditorcommand.h"
#include <memory>
#include <stack>

using namespace Editor::Commands;
using namespace std;

struct EditorUndoHandler::Impl{
    /*
     * Todo: Make undo Tree one day
     */

    stack<unique_ptr<IEditorCommand>> undoStack;
    stack<unique_ptr<IEditorCommand>> redoStack;

    void Undo(){
        if(undoStack.empty())
            return;

        unique_ptr<IEditorCommand> command = std::move(undoStack.top());
        undoStack.pop();

        command->Undo();
        redoStack.push(std::move(command));
    }

    void Redo(){
        if(redoStack.empty())
            return;

        unique_ptr<IEditorCommand> command = std::move(redoStack.top());
        redoStack.pop();

        command->Do();
        undoStack.push(std::move(command)); 
    }

    void AddCommand(std::unique_ptr<IEditorCommand> command){
        undoStack.push(std::move(command));
        redoStack = {};
    }
};

EditorUndoHandler::EditorUndoHandler() : pImpl(std::make_unique<Impl>()){};
EditorUndoHandler::~EditorUndoHandler() = default;

void EditorUndoHandler::UndoCommand(){
    pImpl->Undo();
}

void EditorUndoHandler::RedoCommand(){
    pImpl->Redo();
}

void EditorUndoHandler::AddCommand(std::unique_ptr<IEditorCommand> command){
    pImpl->AddCommand(std::move(command));
}
