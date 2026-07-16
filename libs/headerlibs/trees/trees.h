#pragma once

#include <array>
#include <vector>
#include <memory>

using namespace std;

namespace Trees{
    constexpr int NDegree = 0;
    constexpr int Binary = 2;

    template<typename Node>
    concept Is_Node = requires(Node node){
        node.get();
        node->branches;
    };

    template<typename Node>
    concept Is_N_Node = requires (Node node) {
        Node::NTreeNode;
    };

    template<typename Node>
    concept Is_Binary_Node = requires(Node node){
        Node::BinaryTreeNode;
        node->left;
        node->right;
    };

    template<typename Node>
    concept Is_Nary_Node = requires(Node node){
        Node::NaryTreeNode;
    };

    // It could be cool to use reflection here to add an alias field for each branch
    template<typename T, int Degree>
    class TreeNode final : private enable_shared_from_this<TreeNode<T, Degree>>{
        public:
            struct NTreeNode{};
            weak_ptr<TreeNode> parent{};

            T value{};
            array<shared_ptr<TreeNode>, Degree> branches{nullptr};

            TreeNode(T&& value) : value(std::move(value)){};
            TreeNode(T&& value, weak_ptr<TreeNode> parent) : value(std::move(value)), parent(parent){};

            template <int Index>
            std::shared_ptr<TreeNode> SetBranch(T value){
                std::shared_ptr<TreeNode> newNode = make_shared<TreeNode>(move(value), this->weak_from_this());
 
                branches[Index] = newNode;
                return move(newNode);
            }

            std::shared_ptr<TreeNode> SetBranch(T value, int index){
                std::shared_ptr<TreeNode> newNode = make_shared<TreeNode>(move(value), this->weak_from_this());
 
                branches[index] = newNode;
                return move(newNode);
            }
    };

    template<typename T>
    class TreeNode<T, Binary> final : private enable_shared_from_this<TreeNode<T, Binary>>{
        public:
            struct BinaryTreeNode{};
            T value{};

            weak_ptr<TreeNode> parent{};
            array<shared_ptr<TreeNode>, Binary> branches{nullptr};

            shared_ptr<TreeNode>& left = branches[0];
            shared_ptr<TreeNode>& right = branches[1];

            TreeNode(T&& value) : value(std::move(value)){};
            TreeNode(T&& value, weak_ptr<TreeNode> parent) : value(std::move(value)), parent(parent){};

            shared_ptr<TreeNode> SetLeftBranch(T value){
                shared_ptr<TreeNode> newNode = make_shared<TreeNode>(move(value), this->weak_from_this());
                left = newNode; 
                return move(newNode);
            }

            shared_ptr<TreeNode> SetRightBranch(T value){
                shared_ptr<TreeNode> newNode = make_shared<TreeNode>(move(value), this->weak_from_this());
                right = newNode; 
                return move(newNode);
            }
    };

    template<typename T>
    class TreeNode<T, NDegree> final : private enable_shared_from_this<TreeNode<T, NDegree>>{
        public:
            struct NaryTreeNode{};
            T value{};

            weak_ptr<TreeNode> parent{};
            vector<shared_ptr<TreeNode>> branches{};

            TreeNode(T&& value) : value(std::move(value)){};
            TreeNode(T&& value, weak_ptr<TreeNode> parent) : value(std::move(value)), parent(parent){};

            shared_ptr<TreeNode> AddBranch(T value){
                shared_ptr<TreeNode> newNode = make_shared<TreeNode>(move(value), this->weak_from_this());
                branches.push_back(newNode);
                return move(newNode);
            }
    };
     
    template<typename T, int N>
    class Tree final{
        public:
            using Node = TreeNode<T, N>;
            using NodePtr = shared_ptr<Node>;

            NodePtr rootNode;

            explicit Tree(T&& defaultValue) noexcept{
                rootNode = make_shared<Node>(std::move(defaultValue));
            }
    };

    template<typename T>
    using BinaryTree = Tree<T, Binary>;

    template<typename T>
    using NaryTree = Tree<T, NDegree>;
}
