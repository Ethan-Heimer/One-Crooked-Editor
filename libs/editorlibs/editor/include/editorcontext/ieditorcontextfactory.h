#pragma once

#include "ieditorcontext.h"

namespace Editor {
    class IEditorContextFactory{
        public:
            constexpr IEditorContextFactory(){};

            virtual std::shared_ptr<IEditorContext> 
                Instanciate(queue<int>* inputQueue, string fileName) const = 0;
    };
}
