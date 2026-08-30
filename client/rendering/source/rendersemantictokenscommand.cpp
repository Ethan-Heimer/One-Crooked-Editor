#include "editorrenderingstate.hpp"
#include "rendercommandbase.hpp"
#include "renderlspsemantictokens.hpp"
#include "utils/token.h"
#include <array>
#include <map>
#include <vector>

#define WHITE {240, 240, 240}
#define RED {184, 91, 106}
#define REDORANGE {244, 125, 117}
#define ORANGE {250, 187, 134}
#define LIGHTORANGE {255, 224, 175}
#define GREEN {205, 254, 185}
#define DARKBLUE {98, 130, 160}
#define BLUE {114, 216, 249}
#define LIGHTBLUE {182, 224, 246}
#define PINK {213, 180, 197}
#define PURPLE {127, 65, 99}


using namespace CrookedEditor::Renderer;
    // There needs to be an Adapter, API layer between the LSP and editor functions.
    // The editor client will basically say "hey, variables are blue" and the 
    // LSP will say "variables are represented with the int 5"
    // therefor the connection will be made, a token of 5 means blue text. 
    //
    // currently however, this is the token clangd supports, and the colors
    // are hard coded. I dont think i can achieve this API just yet, as this type
    // of configuration seems like somthing the lua could handle well. 
    //
    // anyway, crooked editor just needs some sort of configuration layer. 

    enum class TokenTypes : int{
        Variable = 0,
        Variable2,
        Parameter,
        Function,
        Method, 
        Function2,
        Property,
        Variable3,
        Class,
        Interface,
        Enum,
        EnumMember,
        Type,
        Type2,
        Unknown,
        Namespace,
        TypeParameter,
        Concept,
        Type3,
        Macro,
        Modifier,
        Operator,
        Bracket,
        Lable,
        Comment
    };

    std::map<TokenTypes, std::array<unsigned char, 3>> colors;

RenderSemanticTokensCommand::RenderSemanticTokensCommand(const Terminal::TerminalController& terminalController, const std::vector<int>& tokens, const EditorRenderingState& editorRenderingState)
    : RenderCommandBase(terminalController), tokens(tokens), editorRenderingState(editorRenderingState){
        colors[TokenTypes::Variable] = BLUE;
        colors[TokenTypes::Variable2] = BLUE;
        colors[TokenTypes::Parameter] = PURPLE;
        colors[TokenTypes::Function] = ORANGE;
        colors[TokenTypes::Method] = LIGHTORANGE;
        colors[TokenTypes::Function2] = ORANGE;
        colors[TokenTypes::Property] = PURPLE;
        colors[TokenTypes::Variable3] = BLUE;
        colors[TokenTypes::Class] = DARKBLUE;
        colors[TokenTypes::Interface] = LIGHTBLUE;
        colors[TokenTypes::Enum] = REDORANGE;
        colors[TokenTypes::EnumMember] = REDORANGE;
        colors[TokenTypes::Type] = RED;
        colors[TokenTypes::Type2] = RED;
        colors[TokenTypes::Unknown] = WHITE;
        colors[TokenTypes::Namespace] = PURPLE;
        colors[TokenTypes::TypeParameter] = PINK;
        colors[TokenTypes::Concept] = PURPLE;
        colors[TokenTypes::Type3] = RED;
        colors[TokenTypes::Macro] = PINK;
        colors[TokenTypes::Modifier] = WHITE;
        colors[TokenTypes::Operator] = WHITE;
        colors[TokenTypes::Bracket] = WHITE;
        colors[TokenTypes::Lable] = GREEN;
        colors[TokenTypes::Comment] = GREEN;
    };

void RenderSemanticTokensCommand::Do(Rendering::CursorPosition& cursorPos, Rendering::TUITexture& frameBuffer){
    int lineDelta{};
    int columnDelta{};
    int length{};
    int tokenType{};
    int attrBitMask{};

    for(int i = 0; i < tokens.size(); i += 5){
        //we should be checking that these tokens are in the bounds of the vector
        lineDelta += tokens[i];
        if(lineDelta < editorRenderingState.firstLineNumberShown)
            continue;
        else if(lineDelta > editorRenderingState.lastLineNumberShown)
            break;;

        bool sameLine = tokens[i] == 0;
        if(sameLine)
            columnDelta += tokens[i+1];
        else
            columnDelta = tokens[i+1];

        length = tokens[i+2];
        tokenType = tokens[i+3];
        attrBitMask = tokens[i+4];

        for(int j = 0; j < length; j++){
            //make some way to edit pixel info
            
            int column = columnDelta + j + 4 - editorRenderingState.colOffset;
            if(column < 4)
                continue;

            auto pixelData = frameBuffer.GetPixel(column, lineDelta - editorRenderingState.firstLineNumberShown);
            std::array<unsigned char, 3> color = colors[static_cast<TokenTypes>(tokenType)];
            frameBuffer.SetPixel(column, lineDelta - editorRenderingState.firstLineNumberShown, {pixelData.character, color[0], color[1], color[2]});
        }
    }
}
