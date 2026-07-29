#pragma once

#include "ieditable.h"
#include <functional>
#include <memory>
namespace Editor::Commands{
    class Command final{
        public:
            template<typename T>
            Command(std::function<void(Command)> addToUndo, T value) 
            : addToUndo(addToUndo), impl(std::make_unique<Model<T>>(std::move(value))){}

            Command(Command&& other){
                impl = std::move(other.impl);
                addToUndo = other.addToUndo;

                other.addToUndo = {};
            }

            void Execute(){
                impl->Initialize();
                impl->Do();
                addToUndo(Copy());
            }

            void Undo(){
                impl->Undo();
            }

            void Do(){
                impl->Do();
            }

            Command Copy() const{
                return impl->Copy(*this);
            }

            void operator()(){
                Execute();
            }

        private:
            struct Contract{
                virtual void Initialize() = 0;
                virtual void Undo() = 0;
                virtual void Do() = 0;

                virtual Command Copy(const Command& other) const = 0;

                virtual ~Contract() = default;
            };

            template<typename T>
            struct Model : Contract{
                T value;
                Model(T value) : value(std::move(value)) {}

                void Initialize() override{
                    value.Initialize();
                }

                void Do() override{
                    value.Do();
                }

                void Undo() override{
                    value.Undo();
                }

                Command Copy(const Command& other) const{
                    return Command{other.addToUndo, T{value}};
                }
            };

            std::unique_ptr<Contract> impl;

            std::function<void(Command command)> addToUndo;
            std::weak_ptr<IEditable> buffer;
    };
}
