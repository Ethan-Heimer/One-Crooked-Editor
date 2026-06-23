#pragma once

#include <memory>
#include <type_traits>

#include "buffer.h"
#include "iinputmanager.h"
#include "inputmanager.h"

using namespace std;

namespace Editor{
    class EditorFactory;
    class EditorPasskey{
        private:
            EditorPasskey(){};
            friend class EditorFactory;
    };

    class IEditor{
        public:
            bool quit;

            IEditor(EditorPasskey passkey){}

            shared_ptr<Buffers::Buffer> buffer;
            weak_ptr<Systems::Input::IInputManager> inputManager;

            virtual void Initialize() = 0;
            virtual void Save() = 0;
            virtual void Update() = 0;

        protected:
            string fileName;

    };

    class EditorFactory{
        public:
            template<typename T, typename... U> 
            requires std::is_base_of<IEditor, T>::value
            std::shared_ptr<IEditor> Instanciate(weak_ptr<Systems::Input::IInputManager> inputManager, string fileName, U... args){
                std::shared_ptr<IEditor> pointer = std::make_shared<T>(EditorPasskey{}, inputManager, fileName, args...);
                pointer->Initialize();

                return std::move(pointer);
            }

    };

}
