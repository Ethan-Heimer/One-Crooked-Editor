#pragma once

#include <memory>
#include <ieditable.h>
#include "editormutatortype.h"
#include "editorundohandler.h"
#include "ieditormutatorbehavior.h"

namespace Editor::Mutators {
    class MutatorManager {
        public:
            MutatorManager(UndoHandler& undoHandler) : undoHandler(undoHandler){};

            void Initialize(std::weak_ptr<IEditable> buffer){
                this->buffer = buffer;
            };


            template<typename T, typename... U>
            requires std::is_base_of_v<IMutatorBehavior, T> 
            T* RegesterMutation(U... args){
                auto addToUndo = [this](Mutator command){
                    this->undoHandler.AddMutator(std::move(command));
                };

                Mutator command{addToUndo, T{buffer, args...}};
                T* ptr = static_cast<T*>(command.PtrToBehavior());
                this->undoHandler.AddMutator(std::move(command));

                return ptr;
            }

        private:
            UndoHandler& undoHandler;
            std::weak_ptr<IEditable> buffer;
    };
}
