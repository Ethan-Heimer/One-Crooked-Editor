#pragma once

#include "requestbus.hpp"

#include <expected>
#include <sstream>
#include <vector>

namespace Events{
    class ResultBatch{
        public:
            std::vector<std::expected<void, std::string>> errors;

            void operator=(std::expected<void, std::string>&& result){
                if(!result.has_value())
                    errors.push_back(result);
            }

            std::string GetErrorMessages() const{
                std::stringstream stream;
                for(auto& o : errors){
                    if(!o.has_value())
                        stream << o.error() << std::endl;
                }

                return stream.str();
            }

            bool HasError() const{
                return !errors.empty();
            }

            template <typename T>
            void Log(Events::RequestBus& bus){
                if(HasError())
                    auto _ = bus.SendRequest<T>(GetErrorMessages());

                errors.clear();
            }
    };
}
