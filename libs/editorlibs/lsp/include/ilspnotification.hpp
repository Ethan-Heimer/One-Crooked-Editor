#pragma once

#include <string>

namespace LSP{
    class ILSPNotification{
        public:
            virtual std::string GetJson() const = 0;
    };
}
