#include "trees.h"
#include <memory>
#include <string>
#include <print>
#include "treealgorithms.h"

using namespace Trees;



int main(){
    NaryTree<std::string> tree{"Root"};
    BinaryTree<std::string> binaryTree{"Root"};

    auto rootNode = tree.rootNode;

    auto node1 = rootNode->AddBranch("1");
    auto node2 = rootNode->AddBranch("2");
    auto node3 = rootNode->AddBranch("3");

    auto node4 = node1->AddBranch("4");
    auto node5 = node1->AddBranch("5");
    auto node6 = node1->AddBranch("6");

    auto node7 = node2->AddBranch("7");
    auto node8 = node2->AddBranch("8");
    auto node9 = node2->AddBranch("9");

    auto node10 = node9->AddBranch("10");
    auto node11 = node9->AddBranch("11");
    auto node12 = node9->AddBranch("12");

    auto res = Algorithms::BreadthFirstSearch(rootNode, "11");
    if(res != nullptr){
        std::println("found: {}", res->value);
    }

    res = Algorithms::FindInChildren(rootNode, "34");
    if(res != nullptr){
        std::println("found: {}", res->value);
    }

    auto rootNode2 = binaryTree.rootNode;

    auto bNode1 = rootNode2->SetLeftBranch("Left 1");
    auto bNode2 = rootNode2->SetRightBranch("Right 1");

    auto bNode3 = bNode1->SetLeftBranch("Left 2");
    auto bNode4 = bNode2->SetRightBranch("Right 2");

    auto res2 = Algorithms::BreadthFirstSearch(rootNode2, "Right 2");
    if(res2 != nullptr){
        std::println("found: {}", res2->value);
    }
}
