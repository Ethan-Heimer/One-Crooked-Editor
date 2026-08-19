#pragma once

#include "ilspresponse.hpp"
#include <memory>
#include <string>
#include <type_traits>

namespace LSP{
    template<typename T>
    requires std::is_base_of_v<ILSPResponse, T>
    class ILSPRequest{
        public:
            virtual std::string GetJson(int requestId) const = 0;

            T GetResponseType() const{
                return T{};
            }
    };
}
