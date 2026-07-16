#include "editoractiontree.h"
#include "trees.h"
#include "treealgorithms.h"
#include <memory>
#include <utility>

using namespace Editor::Actions;
using namespace Trees;
using namespace Trees::Algorithms;

struct KeyStroke{
    char stroke{};
    std::unique_ptr<ActionBase> action{};

    KeyStroke(char stroke, std::unique_ptr<ActionBase> action) : 
        stroke(stroke), action(std::move(action)){}

    KeyStroke(const KeyStroke& other) = delete;
    KeyStroke(KeyStroke&& other){
        stroke = other.stroke;
        action = std::move(action);

        other.stroke = '\0';
    }

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

    KeyStroke rootValue{' ', nullptr};
    StrokeTree actionTree{std::move(rootValue)};

    StrokeTree::NodePtr currentKeyStroke{actionTree.rootNode};

    void AppendAction(string strokes, unique_ptr<ActionBase> action){
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

        if(nextNode == nullptr){
            KeyStroke node{stroke, nullptr};
            nextNode = currentNode->AddBranch(std::move(node));
        }

        AddAction(nextNode, strokes.substr(1, strokes.length()), std::move(action));
    }

    void InvokeNextAction(char key){
        StrokeTree::NodePtr nextNode = FindInChildren(currentKeyStroke, key);
        if(nextNode == nullptr){
            ResetCurrentAction();
            return;
        }

        nextNode->value.Invoke();
        currentKeyStroke = nextNode;

        if(currentKeyStroke->branches.empty()){
            ResetCurrentAction();
            return;
        };
    };

    void ResetCurrentAction(){ 
        currentKeyStroke = actionTree.rootNode;   
    };
};

ActionTree::ActionTree() : pImpl(std::make_unique<ActionTree::Impl>()){};
ActionTree::~ActionTree(){}

ActionTree& ActionTree::AppendAction(string strokes, unique_ptr<ActionBase> action){
    pImpl->AppendAction(strokes, std::move(action));
    return *this;
}

void ActionTree::TraverseToNextAction(char key){
    pImpl->InvokeNextAction(key);
}
