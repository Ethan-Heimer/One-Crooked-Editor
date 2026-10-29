#pragma once

#include "application.hpp"
#include "configvault.hpp"
#include "events.hpp"
#include "hooks.hpp"

#include "terminal.hpp"
#include "editorvisualstate.hpp"

#include <memory>

namespace CrookedEditor::Application{
    class TextEditor{
        public:
            using CycleCompleteEventType = Events::Event<TextEditor, const std::shared_ptr<const Components::TextEditorVisualState>>;
            using InputHookType = Events::SoliatryHook<TextEditor, int>;

            CycleCompleteEventType OnEditorCycleComplete;
            InputHookType SGetNextInputHook;

            TextEditor(Application& app, Terminal::TerminalController& terminalController, ConfigVault& config);
            ~TextEditor();

            void Start();
            void Stop();

        private:
            struct Impl;
            std::unique_ptr<Impl> pImpl;
    };
}
