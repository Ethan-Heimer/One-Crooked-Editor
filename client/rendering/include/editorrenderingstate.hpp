#pragma once

namespace CrookedEditor::Renderer{
    struct EditorRenderingState{
        int lastRowNumber;
        int currentLineViewOffset;
        int colOffset;

        int firstLineNumberShown;
        int lastLineNumberShown;
    };

}
