#pragma once

#include <map>
#include <memory>
#include <queue>
#include <string_view>

#include "editorfilehandling/ieditorfilehandler.h"
#include "ieditable.h"
#include "editorcommandmanager.h"
#include "ieditorstate.h"
#include "statemachine.h"

namespace Editor::States{
    class StateContext final : public StateMachines::BaseStateContext{
        public:
            std::map<std::string, std::shared_ptr<IEditorState>> states;

            FileHandling::FileHandler& fileSaver;
            std::weak_ptr<IEditable> buffer;
            Commands::CommandManager& commandManager;
            Commands::UndoHandler& undoHandler;

            std::queue<int>* inputQueue;
            bool* quitToken;

            StateContext(Commands::CommandManager& commandManager, FileHandling::FileHandler& fileHandler, Commands::UndoHandler& undoHandler) :
            commandManager(commandManager), fileSaver(fileHandler), undoHandler(undoHandler){};

            template<typename S>
            requires std::is_base_of_v<IEditorState, S>
            void AddState(){
                std::shared_ptr<IEditorState> newState = 
                    std::make_shared<S>(fileSaver, buffer, *this, commandManager, undoHandler, inputQueue, quitToken);
                states[newState->StateName()] = std::move(newState);
            }

            void ChangeState(std::string_view name) override;
            void Initialize(std::weak_ptr<IEditable> buffer, 
                    const std::string& defaultState, 
                    std::queue<int>* inputQueue, bool* quitToken);
            void Update();

            private:
                std::unique_ptr<StateMachines::StateMachine<IEditorState>> stateMachine;
    };
}
