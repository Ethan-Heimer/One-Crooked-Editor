#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace FileHandling{
    class IFileLoadable{
        public:
            virtual void ReadLineFromFile(const std::string& line) = 0;
    };

    class IFileSaveable{
        public:
            virtual std::stringstream WriteLinesToFile() = 0;
    };

    class IFileLoader{
        public:
            virtual std::shared_ptr<IFileLoadable> LoadFromFile() = 0;
    };

    class IFileSaver{
        public:
            virtual void SaveToFile(const std::shared_ptr<IFileSaveable> fileSaver) = 0;
    };
}
