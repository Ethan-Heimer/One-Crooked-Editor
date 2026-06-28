#pragma once

#include <memory>
#include <queue>
#include "ieditorcontext.h"

using namespace std;
using namespace Editor::Buffers;

namespace Editor{
    class EditorContext : public IEditorContext{
        public:
            using IEditorContext::IEditorContext;

            virtual void Initialize( 
                    shared_ptr<Buffers::IBufferFileHandlerFactory> bufferFileHandlerFactory,
                    shared_ptr<States::IStateContextFactory> stateContextFactory, queue<int>* inputQueue, string fileName) override;

            void Update() override;
            std::weak_ptr<IEditorContext> GetWeakPtr();
    };
}
