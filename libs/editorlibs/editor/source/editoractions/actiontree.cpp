#include "editoractiontree.h"
#include "trees.h"
#include "treealgorithms.h"
#include <memory>

using namespace Editor::Actions;
using namespace Trees;
using namespace Trees::Algorithms;

struct KeyStroke{
    char stroke{};
    std::unique_ptr<ActionBase> action{};

    bool operator==(const KeyStroke& other){
        return this->stroke == other.stroke;
    };

    bool operator!=(const KeyStroke& other){
        return this->stroke != other.stroke;
    };

    bool operator==(const char& stroke){
        return this->stroke == stroke;
    };

    bool operator!=(const char& stroke){
        return this->stroke != stroke;
    };

    void Invoke(){
        if(action == nullptr)
            return;

        action->Invoke();
    }
};

struct ActionTree::Impl{
    using StrokeTree = Tree<KeyStroke, NDegree>;

    StrokeTree actionTree{KeyStroke{' ', nullptr}};
    StrokeTree::NodePtr currentKeyStroke{actionTree.rootNode};

    void AddAction(string strokes, unique_ptr<ActionBase> action){
        AddAction(actionTree.rootNode, strokes, std::move(action));
    };

    void AddAction(StrokeTree::NodePtr currentNode, string strokes, unique_ptr<ActionBase> action){
        if(strokes.empty()){
            currentNode->value.action = std::move(action);
            return;
        }
        
        char stroke = strokes[0];
        StrokeTree::NodePtr nextNode = nullptr;

        if(!currentNode->branches.empty())
            nextNode = FindInChildren(currentNode, stroke);

        if(nextNode == nullptr)
            nextNode = currentNode->AddBranch({stroke, nullptr});

        AddAction(nextNode, strokes.substr(1, strokes.length()), std::move(action));
    }

    void InvokeNextAction(char key){
        if(currentKeyStroke->branches.empty()){
            ResetCurrentAction();
            return;
        };

        StrokeTree::NodePtr nextNode = FindInChildren(currentKeyStroke, key);
        if(nextNode == nullptr){
            ResetCurrentAction();
            return;
        }

        nextNode->value.Invoke();
        currentKeyStroke = nextNode;
    };

    void ResetCurrentAction(){ 
        currentKeyStroke = actionTree.rootNode;   
    };
};
