#pragma once

#include "ieditable.h"
#include <memory>
#include <utility>

namespace Editor::FileHandling {
    class FileHandler{
        public:
            std::string fileName;

            template<typename T>
            FileHandler(T fileInterpretor) 
            : impl(std::make_unique<Model<T>>(std::move(fileInterpretor))){};

            std::shared_ptr<Editor::IEditable> LoadFromFile(){
                return impl->LoadFromFile(fileName);
            };
            
            void SaveToFile(const IEditable& buffer){
                return impl->SaveToFile(fileName, buffer);
            };

        private:
            struct Contract{
               virtual ~Contract() = default; 

                virtual std::shared_ptr<Editor::IEditable> LoadFromFile(std::string_view fileName) = 0;
                virtual void SaveToFile(std::string_view fileName, const IEditable& fileSaver) = 0;
            };

            template <typename T>
            struct Model : Contract{
                T interpreter;
                Model(T interpreter) : interpreter(std::move(interpreter)) {};

                std::shared_ptr<Editor::IEditable> LoadFromFile(std::string_view fileName) override{
                    return interpreter.LoadFromFile(fileName);
                };

                void SaveToFile(std::string_view fileName, const IEditable& fileSaver) override{
                    return interpreter.SaveToFile(fileName, fileSaver);
                }
            };

            std::unique_ptr<Contract> impl;
    };
}
