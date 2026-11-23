#pragma once

#include "requests.hpp"
#include <utility>

namespace Events{
    template<typename Ret, typename... Params>
    class RequestChannel{
        public:
            using RequestType = Request<RequestChannel, Ret, Params...>;
            RequestType request;

            void On(typename RequestType::request_callback_type&& listener){
                request.On(std::move(listener));
            }
    
            auto Send(Params&&... args) -> typename RequestType::request_return_type{
                return request.Send(std::forward<Params>(args)...);
            }
   
    };

}
