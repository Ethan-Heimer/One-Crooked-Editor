#include "editoractiontree.h"
#include "trees.h"
#include "treealgorithms.h"
#include <memory>
#include <optional>
#include <utility>

using namespace Editor::Actions;
using namespace Trees;
using namespace Trees::Algorithms;

struct KeyStroke{
    char stroke{};
    std::optional<Action> action{};

    KeyStroke(char stroke) : stroke(stroke){};
    KeyStroke(char stroke, Action action) : 
        stroke(stroke), action(std::move(action)){}

    KeyStroke(const KeyStroke& other) = delete;
    KeyStroke(KeyStroke&& other) : action(std::move(other.action)){
        stroke = other.stroke;

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
        if(action.has_value())
            action->Invoke();
    }
};

struct ActionTree::Impl{
    using StrokeTree = Tree<KeyStroke, NDegree>;

    KeyStroke rootValue{' '};
    StrokeTree actionTree{std::move(rootValue)};

    StrokeTree::NodePtr currentKeyStroke{actionTree.rootNode};

    void AppendAction(string strokes, Action action){
        AddAction(actionTree.rootNode, strokes, std::move(action));
    };

    void AddAction(StrokeTree::NodePtr currentNode, string strokes, Action action){
        if(strokes.empty()){
            currentNode->value.action = std::move(action);
            return;
        }
        
        char stroke = strokes[0];
        StrokeTree::NodePtr nextNode = nullptr;

        if(!currentNode->branches.empty())
            nextNode = FindInChildren(currentNode, stroke);

        if(nextNode == nullptr){
            KeyStroke node{stroke};
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

ActionTree& ActionTree::AddAction(string strokes, Action action){
    pImpl->AppendAction(strokes, std::move(action));
    return *this;
}

void ActionTree::TraverseToNextAction(char key){
    pImpl->InvokeNextAction(key);
}
