#pragma once

#include "ieditorcontext.h"

namespace Editor {
    template<typename T>
    requires std::is_base_of_v<IEditable, T>
    class IEditorContextFactory{
        public:
            constexpr IEditorContextFactory(){};

            virtual std::shared_ptr<IEditorContext<T>> 
                Instanciate(queue<int>* inputQueue, string fileName) const = 0;
    };
}
