#include "editorundohandler.h"

#include "editorcommands/ieditorcommand.h"
#include <memory>
#include <stack>

using namespace Editor::Commands;
using namespace std;

struct UndoHandler::Impl{
    /*
     * Todo: Make undo Tree one day
     */

    stack<Command> undoStack;
    stack<Command> redoStack;

    void Undo(){
        if(undoStack.empty())
            return;

        Command command = std::move(undoStack.top());
        undoStack.pop();

        command.Undo();
        redoStack.push(std::move(command));
    }

    void Redo(){
        if(redoStack.empty())
            return;

        Command command = std::move(redoStack.top());
        redoStack.pop();

        command.Do();
        undoStack.push(std::move(command)); 
    }

    void AddCommand(Command command){
        undoStack.push(std::move(command));
        redoStack = {};
    }
};

UndoHandler::UndoHandler() : pImpl(std::make_unique<Impl>()){};
UndoHandler::~UndoHandler() = default;

void UndoHandler::UndoCommand(){
    pImpl->Undo();
}

void UndoHandler::RedoCommand(){
    pImpl->Redo();
}

void UndoHandler::AddCommand(Command command){
    pImpl->AddCommand(std::move(command));
}
