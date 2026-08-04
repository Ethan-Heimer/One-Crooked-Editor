#pragma once

#include "editormutations/editormutatormanager.h"
#include "editormutations/editorundohandler.h"
#include "ieditorstate.h"
#include <functional>
#include <memory>
#include <string_view>

namespace Editor::States{
    struct StateConstructorArgs{
        IEditable& buffer;
        std::function<void(std::string_view)> switchState;
        std::function<void()> saveBuffer;
        Mutators::MutatorManager& commandManager;
        Mutators::UndoHandler& undoHandler;
        std::queue<int>* inputQueue;
        bool* quitToken;
    };

    template<typename T>
    struct StateType{
        std::shared_ptr<IEditorState> Instanciate(StateConstructorArgs args){
            return std::make_shared<T>(args.buffer, args.saveBuffer, args.switchState,
                    args.commandManager, args.undoHandler, args.inputQueue, args.quitToken);
        };
    };

    struct StateTypeValue{
        public:
            template <typename T>
            StateTypeValue(T value) : impl(std::make_unique<Model<T>>(std::move(value))){}

            std::shared_ptr<IEditorState> Instanciate(StateConstructorArgs args){
                return impl->Instanciate(args);
            }

        private:
            struct Contract{
                virtual ~Contract() = default;
                virtual std::shared_ptr<IEditorState> Instanciate(StateConstructorArgs args) = 0;
            };

            template<typename T>
            struct Model : Contract{
                T value;
                Model(T value) : value(std::move(value)){}

                std::shared_ptr<IEditorState> Instanciate(StateConstructorArgs args) override{
                    return value.Instanciate(args);
                }
            };

            std::unique_ptr<Contract> impl;
    };
}
