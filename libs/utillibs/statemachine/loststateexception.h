#pragma once

#include <exception>
#include <string>

namespace StateMachines{
    class LostStateException : public std::exception{
        public:
            LostStateException(const std::string message) : message(message){};
            const char* what() const noexcept {
                return message.c_str(); 
            }

        private: 
            const std::string message;
    };
}
