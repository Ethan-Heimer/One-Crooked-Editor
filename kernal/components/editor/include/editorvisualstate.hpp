#pragma once

#include <vector>
namespace CrookedEditor::Components{
    struct TextEditorVisualState{
        int lastRowNumber;
        int currentLineViewOffset;

        int colOffset;
        int cursorCol;

        int firstLineNumberShown;
        int lastLineNumberShown;

        int lineNumberPaddingWidth;
        int lineNumberColumnWidth;

        std::vector<std::string> lines;
        std::vector<std::string> lineNumbers;

        TextEditorVisualState Dispatch(){
            TextEditorVisualState temp{
                 lastRowNumber,
                 currentLineViewOffset,

                 colOffset,
                 cursorCol,

                 firstLineNumberShown,
                 lastLineNumberShown,

                 lineNumberPaddingWidth,
                 lineNumberColumnWidth,

                 std::move(lines),
                 std::move(lineNumbers)
            };

            lines = {};
            lineNumbers = {};

            return temp;
        }
    };

}
