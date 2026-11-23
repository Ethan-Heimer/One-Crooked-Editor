#pragma once

#include "ieditable.h"
#include <cassert>
#include <memory>
#include <utility>

namespace Editor::FileHandling {
    class FileHandler{
        public:
            std::string fileName;

            FileHandler(){
            };

            template<typename T>
            FileHandler(T fileInterpretor) 
            : impl(std::make_unique<Model<T>>(std::move(fileInterpretor))){
            };

            FileHandler(const FileHandler& other) = delete;
            FileHandler(FileHandler&& other){
                this->fileName = std::move(other.fileName);
                this->impl = std::move(other.impl);

            }

            FileHandler& operator=(const FileHandler& other) = delete;
            FileHandler& operator=(FileHandler&& other){
                this->fileName = std::move(other.fileName);
                this->impl = std::move(other.impl);

                return *this;
            }

            std::shared_ptr<Editor::IEditable> LoadFromFile(){
                assert(impl && "No Impl In Load From File");

                return impl->LoadFromFile(fileName);
            };
            
            void SaveToFile(const IEditable& buffer){
                assert(impl && "No Impl In Save File");

                impl->SaveToFile(fileName, buffer);
            };

        private:
            struct Contract{
               virtual ~Contract() = default; 

                virtual std::shared_ptr<Editor::IEditable> LoadFromFile(std::string_view fileName) = 0;
                virtual void SaveToFile(std::string_view fileName, const IEditable& fileSaver) = 0;
                virtual void Test() = 0;
            };

            template <typename T>
            struct Model : public Contract{
                T interpreter;
                Model(T interpreter) : interpreter(std::move(interpreter)) {};

                std::shared_ptr<Editor::IEditable> LoadFromFile(std::string_view fileName) override{
                    return interpreter.LoadFromFile(fileName);
                };

                void SaveToFile(std::string_view fileName, const IEditable& fileSaver) override{ 
                    assert(fileName.length() > 0 && "No File Given");

                    return interpreter.SaveToFile(fileName, fileSaver);
                }

                void Test() override{
                    std::cout << "Test" << std::endl;
                };
            };

            std::unique_ptr<Contract> impl{};
    };
}
