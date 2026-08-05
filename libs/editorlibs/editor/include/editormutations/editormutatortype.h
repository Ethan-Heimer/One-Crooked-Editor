#pragma once

#include "ieditable.h"
#include <functional>
#include <memory>
namespace Editor::Mutators{
    class Mutator final{
        public:
            template<typename T>
            Mutator(std::function<void(Mutator)> addToUndo, T&& value) 
            : addToUndo(addToUndo), impl(std::make_unique<Model<T>>(std::move(value))){}

            Mutator(Mutator&& other){
                impl = std::move(other.impl);
                addToUndo = other.addToUndo;

                other.addToUndo = {};
            }

            void Undo(){
                impl->Undo();
            }

            void Do(){
                impl->Do();
            }

            Mutator Copy() const{
                return impl->Copy(*this);
            }

            void* PtrToBehavior(){
                return impl->GetPtrToBehavior();
            }

        private:
            struct Contract{
                virtual void Undo() = 0;
                virtual void Do() = 0;

                virtual Mutator Copy(const Mutator& other) const = 0;
                virtual void* GetPtrToBehavior() = 0;

                virtual ~Contract() = default;
            };

            template<typename T>
            struct Model : Contract{
                T value;
                Model(T&& value) : value(std::move(value)) {}

                void Do() override{
                    value.Do();
                }

                void Undo() override{
                    value.Undo();
                }

                Mutator Copy(const Mutator& other) const override{
                    return Mutator{other.addToUndo, T{value}};
                }

                void* GetPtrToBehavior() override{
                    return &value;
                }
            };

            std::unique_ptr<Contract> impl;

            std::function<void(Mutator command)> addToUndo;
            std::weak_ptr<IEditable> buffer;
    };
}
