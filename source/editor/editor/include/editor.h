#pragma once

#include "iinputmanager.h"
#include <memory>
#include "ieditor.h"

using namespace std;
using namespace Editor::Buffers;
using namespace Systems;

namespace Editor{
    class Editor : public IEditor, public std::enable_shared_from_this<IEditor>{
        public:
            using IEditor::IEditor;

            virtual void Initialize(weak_ptr<Systems::Input::IInputManager> inputManager, 
                    string fileName, weak_ptr<Buffers::IBufferFileHandlerFactory> bufferFileHandlerFactory,
                    weak_ptr<States::IStateContextFactory> stateContextFactory) override;

            void Update() override;

            std::weak_ptr<IEditor> GetWeakPtr();
    };
}
