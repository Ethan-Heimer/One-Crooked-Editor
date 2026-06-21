#pragma once

#include "buffer.h"
#include "ieditor.h"
#include "iinputmanager.h"
#include "context.h"
#include <memory>

using namespace std;
using namespace Editor::Buffers;
using namespace Systems;

namespace Editor{
    class Editor : public IEditor, public std::enable_shared_from_this<IEditor>{
        public:
            shared_ptr<States::StateContext> stateContext;

            void Initialize(weak_ptr<Input::IInputManager>, const string fileName);

            void Update();
            std::weak_ptr<IEditor> GetWeakPtr();

            void Save() override;
    };
}
