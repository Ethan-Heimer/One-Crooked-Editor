#pragma once

#include "ieditor.h"
#include "iinputmanager.h"
#include "icontext.h"
#include <memory>

using namespace std;
using namespace Editor::Buffers;
using namespace Systems;

namespace Editor{
    class Editor : public IEditor, public std::enable_shared_from_this<IEditor>{
        public:
            shared_ptr<States::IStateContext> stateContext;
            weak_ptr<States::StateContextFactory> stateContextFactory;

            Editor(EditorPasskey passkey, weak_ptr<Input::IInputManager> inputManager, 
                    string fileName, weak_ptr<States::StateContextFactory> stateContextFactory);

            void Initialize() override;

            void Update() override;
            void Save() override;

            std::weak_ptr<IEditor> GetWeakPtr();
    };
}
