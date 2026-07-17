#include "trees.h"
#include <print>
#include <queue>

namespace Trees::Algorithms{
    template<Is_Node Node, typename Value>
    Node DepthFirstSearch(Node node, Value value){
        if(node == nullptr) return nullptr;
        
        std::println("{}", node->value);
        if(node->value == value)
            return node;

        for(auto nextNode : node->branches) {
            auto res = DepthFirstSearch(nextNode, value);
            if(res != nullptr)
                return res;
        }

        return nullptr;
    }

    template<Is_Node Node, typename Value>
    Node BreadthFirstSearch(Node node, Value value){
        queue<Node> unvisitedNodes;

        unvisitedNodes.push(node);
        while(!unvisitedNodes.empty()){
            auto nextNode = unvisitedNodes.front();
            unvisitedNodes.pop();

            if(nextNode == nullptr)
                continue;

            if(nextNode->value == value)
                return nextNode;

            for(auto node : nextNode->branches){
                unvisitedNodes.push(node);
            }
        }

        return nullptr;
    }

    template<Is_Node Node, typename Value>
    Node FindInChildren(Node node, Value value){
        for(auto node : node->branches){
            if(node->value == value)
                return node;
        }

        return nullptr;
    }
}

