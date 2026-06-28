#pragma once

#include "editorfilehandling/ieditorfilehandlerfactory.h"
#include "ieditorcontextfactory.h"
#include "ieditorstatecontextfactory.h"
#include <memory>
#include <queue>
#include <type_traits>

using namespace Editor::States;
using namespace std;

namespace Editor {
    template <typename E, typename T, typename U, typename V>
    requires std::is_base_of_v<IEditorContext<E>, T> && 
    std::is_base_of_v<IStateContextFactory, U> &&
    std::is_base_of_v<IEditorFileHandlerFactory<E>, V>
    class EditorContextFactory;

    template<typename E, typename T, typename U, typename V>
    class EditorPasskey : public IEditorContextPasskey{
        protected:   
            EditorPasskey() : IEditorContextPasskey(){};
            friend class EditorContextFactory<E, T, U, V>;
    };

    template <typename E, typename T, typename U, typename V>
    requires std::is_base_of_v<IEditorContext<E>, T> &&
    std::is_base_of_v<IStateContextFactory, U> &&
    std::is_base_of_v<IEditorFileHandlerFactory<E>, V>
    class EditorContextFactory : public IEditorContextFactory<E>{
        public:
            using IEditorContextFactory<E>::IEditorContextFactory;

            virtual std::shared_ptr<IEditorContext<E>> 
                Instanciate(queue<int>* inputQueue, string fileName) const override {
                shared_ptr<IEditorContext<E>> pointer = make_shared<T>(EditorPasskey<E, T, U, V>{});
                
                shared_ptr<States::IStateContextFactory> stateContextFactory
                    = make_shared<U>();
                
                shared_ptr<IEditorFileHandlerFactory<E>> bufferFileHandlerFactory
                    = make_shared<V>();

                pointer->Initialize(bufferFileHandlerFactory, stateContextFactory, inputQueue, fileName);

                return std::move(pointer);
                
            };
    };
}
