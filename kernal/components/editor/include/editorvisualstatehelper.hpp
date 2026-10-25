#pragma once

#include "editorvisualstate.hpp"
#include "ieditable.h"

#include "editorcontext.h"
#include "terminal.hpp"
#include "editorvisualstate.hpp"

namespace CrookedEditor::Components{
    class EditorVisualStateHelper{
        public:
            void UpdateVisualState(const Terminal::TerminalController& terminalController, 
            const Editor::EditorContext& context, TextEditorVisualState& visualState, int invisableRows, 
            int verticalScrollThreshold, int lineColumnSpace);

            void ScrollViewVertical(const int& verticalScrollThreshold, const int& visableLines, const int& currentLineNumber, TextEditorVisualState& visualState);
            void ScrollViewHorizontal(const int& lineNumberColumnWidth, const int& termCol, TextEditorVisualState& visualState);
            void GetDataInView(const int& visableLines, TextEditorVisualState& visualState, const Editor::IEditable& buffer);
            void CalculateLineNumberColumnValues(TextEditorVisualState& visualState, const int& minLineNumberColWidth);
    };
}
