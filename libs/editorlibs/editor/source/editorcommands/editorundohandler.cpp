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

    stack<unique_ptr<ICommand>> undoStack;
    stack<unique_ptr<ICommand>> redoStack;

    void Undo(){
        if(undoStack.empty())
            return;

        unique_ptr<ICommand> command = std::move(undoStack.top());
        undoStack.pop();

        command->Undo();
        redoStack.push(std::move(command));
    }

    void Redo(){
        if(redoStack.empty())
            return;

        unique_ptr<ICommand> command = std::move(redoStack.top());
        redoStack.pop();

        command->Do();
        undoStack.push(std::move(command)); 
    }

    void AddCommand(std::unique_ptr<ICommand> command){
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

void UndoHandler::AddCommand(std::unique_ptr<ICommand> command){
    pImpl->AddCommand(std::move(command));
}
