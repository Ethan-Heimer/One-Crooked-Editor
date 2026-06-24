#pragma once

#include "ibufferfilehandlerfactory.h"
#include "ieditorfactory.h"
#include <memory>
#include <type_traits>

namespace Editor {
    template <typename T>
    requires std::is_base_of<IEditor, T>::value
    class EditorFactory;

    template<typename T>
    class EditorPasskey : public IEditorPasskey{
        protected:   
            EditorPasskey() : IEditorPasskey(){};
            friend class EditorFactory<T>;
    };

    template <typename T>
    requires std::is_base_of<IEditor, T>::value
    class EditorFactory : public IEditorFactory{
        public:
            virtual std::shared_ptr<IEditor> 
                Instanciate(weak_ptr<Buffers::IBufferFileHandlerFactory> bufferFileHandlerFactory, weak_ptr<States::IStateContextFactory> stateContextFactory,
                        weak_ptr<Systems::Input::IInputManager> inputManager, string fileName) override {
                std::shared_ptr<IEditor> pointer = std::make_shared<T>(EditorPasskey<T>{});
                pointer->Initialize(inputManager, fileName, bufferFileHandlerFactory, stateContextFactory);

                return std::move(pointer);
                
            };
    };
}
