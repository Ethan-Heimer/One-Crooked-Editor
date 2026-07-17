#pragma once

#include "ieditorcontextfactory.h"
#include <memory>
#include <queue>
#include <type_traits>

using namespace Editor::States;
using namespace std;

namespace Editor {
    template <typename E, typename T>
    requires std::is_base_of_v<IEditorContext<E>, T>
    class EditorContextFactory;

    template<typename E, typename T>
    class EditorPasskey : public IEditorContextPasskey{
        protected:   
            EditorPasskey() : IEditorContextPasskey(){};
            friend class EditorContextFactory<E, T>;
    };

    template <typename E, typename T>
    requires std::is_base_of_v<IEditorContext<E>, T>
    class EditorContextFactory : public IEditorContextFactory<E>{
        public:
            using IEditorContextFactory<E>::IEditorContextFactory;

            virtual std::shared_ptr<IEditorContext<E>> 
                Instanciate(queue<int>* inputQueue, string fileName) const override {

                shared_ptr<IEditorContext<E>> pointer = make_shared<T>(EditorPasskey<E, T>{});
                pointer->Initialize(this->bufferFileHandlerFactory, this->stateContextFactory, 
                        this->commandManagerFactory, this->undoHandlerFactory, inputQueue, fileName);

                return std::move(pointer);
                
            };
    };
}
