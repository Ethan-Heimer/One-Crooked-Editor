#pragma once

#include <istream>
namespace CrookedEditor::Renderer{
    struct EditorRenderingState{
        int lastRowNumber;
        int currentLineViewOffset;

        int colOffset;
        int cursorCol;

        int firstLineNumberShown;
        int lastLineNumberShown;

        int lineNumberPaddingWidth;
        int lineNumberColumnWidth;

    };

}
