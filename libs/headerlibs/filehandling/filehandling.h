#pragma once

#include <expected>
#include <generator>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

namespace FileHandling{
    class IFileLoadable{
        public:
            virtual void ReadLineFromFile(const std::string& line) = 0;
    };

    class IFileSaveable{
        public:
            virtual std::stringstream WriteLinesToFile() = 0;
    };

    template<typename T>
    requires std::is_base_of<IFileLoadable, T>::value
    class IFileLoader{
        public:
            virtual std::shared_ptr<T> LoadFromFile() = 0;
    };

    class IFileSaver{
        public:
            virtual void SaveToFile(const std::shared_ptr<IFileSaveable> fileSaver) = 0;
    };
}
