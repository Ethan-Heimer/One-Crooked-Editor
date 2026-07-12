#pragma once

namespace Undoing {
    class IUndoable{
        public:
            virtual void Undo() = 0;
    };
}
