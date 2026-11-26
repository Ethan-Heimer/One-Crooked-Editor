#include "editormutations/editorundohandler.h"

#include <memory>
#include <stack>

using namespace Editor::Mutators;
using namespace std;

struct UndoHandler::Impl{
    /*
     * Todo: Make undo Tree one day
     */

    stack<Mutator> undoStack;
    stack<Mutator> redoStack;

    void Undo(){
        if(undoStack.empty())
            return;

        Mutator command = std::move(undoStack.top());
        undoStack.pop();

        command.Undo();
        redoStack.push(std::move(command));
    }

    void Redo(){
        if(redoStack.empty())
            return;

        Mutator command = std::move(redoStack.top());
        redoStack.pop();

        command.Do();
        undoStack.push(std::move(command)); 
    }

    void AddMutator(Mutator command){
        undoStack.push(std::move(command));
        redoStack = {};
    }
};

UndoHandler::UndoHandler() : pImpl(std::make_unique<Impl>()){};
UndoHandler::~UndoHandler() = default;

void UndoHandler::UndoMutator(){
    pImpl->Undo();
}

void UndoHandler::RedoMutator(){
    pImpl->Redo();
}

void UndoHandler::AddMutator(Mutator command){
    pImpl->AddMutator(std::move(command));
}
