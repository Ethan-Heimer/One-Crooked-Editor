#pragma once

#include "ibufferfilehandlerfactory.h"
#include "ieditorfactory.h"
#include "ieditorstatecontextfactory.h"
#include <memory>
#include <queue>
#include <type_traits>

using namespace Editor::States;
using namespace std;

namespace Editor {
    template <typename T, typename U, typename V>
    requires std::is_base_of_v<IEditorContext, T> && 
    std::is_base_of_v<IStateContextFactory, U> &&
    std::is_base_of_v<Buffers::IBufferFileHandlerFactory, V>
    class EditorContextFactory;

    template<typename T, typename U, typename V>
    class EditorPasskey : public IEditorContextPasskey{
        protected:   
            EditorPasskey() : IEditorContextPasskey(){};
            friend class EditorContextFactory<T, U, V>;
    };

    template <typename T, typename U, typename V>
    requires std::is_base_of_v<IEditorContext, T> &&
    std::is_base_of_v<IStateContextFactory, U> &&
    std::is_base_of_v<Buffers::IBufferFileHandlerFactory, V>
    class EditorContextFactory : public IEditorContextFactory{
        public:
            using IEditorContextFactory::IEditorContextFactory;

            virtual std::shared_ptr<IEditorContext> 
                Instanciate(queue<int>* inputQueue, string fileName) const override {
                shared_ptr<IEditorContext> pointer = make_shared<T>(EditorPasskey<T, U, V>{});
                
                shared_ptr<States::IStateContextFactory> stateContextFactory
                    = make_shared<U>();
                
                shared_ptr<Buffers::IBufferFileHandlerFactory> bufferFileHandlerFactory
                    = make_shared<V>();

                pointer->Initialize(bufferFileHandlerFactory, stateContextFactory, inputQueue, fileName);

                return std::move(pointer);
                
            };
    };
}
