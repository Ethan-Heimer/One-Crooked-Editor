#pragma once

#include <string>

namespace Editor{
    class IEditableFileCommands{
        public:
            virtual void ReadLineFromFile(const std::string& line) = 0;
            virtual std::stringstream WriteLinesToFile() const = 0 ;             

    };
}
