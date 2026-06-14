#pragma once

#include <memory>

using namespace std;

template <typename T>
class DoublyLinkedList{
    public:
        struct Node{
            template<typename... U>
            Node(U... args) : data(std::make_shared<T>(args...)){}
            Node(){};

            shared_ptr<T> data;

            shared_ptr<Node> next;
            weak_ptr<Node> previous;

            bool operator==(const Node& other) const{
                return data.get() == other.data.get();
            }

            bool operator!=(const Node& other) const{
                return data.get() != other.data.get();
            }

            bool operator==(const T& other) const{
                return *data == other;
            }

            bool operator!=(const T& other) const{
                return *data != other;
            }

            T operator *(){
                return *data;
            }
        };

        struct Iterator{
            public:
                Iterator(std::shared_ptr<Node> node) : currentNode(node){};
                Node operator*() const { return *currentNode; };
                Iterator& operator++() { currentNode = currentNode->next; return *this; };
                friend bool operator!= (const Iterator& a, const Iterator& b) {return a.currentNode != b.currentNode;}

            private:
                std::shared_ptr<Node> currentNode;          
        };

        shared_ptr<Node> head;
        shared_ptr<Node> tail;

        template<typename... U>
        shared_ptr<Node> Append(U... args){
            if(!head){
                head = std::make_shared<Node>(args...);
                tail = head;
            }
            else{
                shared_ptr<Node> newNode = make_shared<Node>(args...);

                tail->next = newNode;
                newNode->previous = tail;

                tail = newNode;
            }

            return tail;
        }

        template<typename... U>
        shared_ptr<Node> AppendBefore(shared_ptr<Node> node, U... args){
            if(!node)
                return nullptr;

            shared_ptr<Node> newNode = make_shared<Node>(args...);

            if(node == head){
                newNode->next = head;
                head->previous = newNode;

                head = newNode;
            }else{
                shared_ptr<Node> previousNode = node->previous.lock();
                if(!previousNode)
                    return nullptr;
                
                previousNode->next = newNode;
                newNode->previous = previousNode;

                node->previous = newNode;
                newNode->next = node;
            }

            return newNode;
        }

        template<typename... U>
        shared_ptr<Node> AppendAfter(shared_ptr<Node> node, U... args){
            if(!node)
                return nullptr;


            if(node == tail){
                return Append(args...);
            } else {
                shared_ptr<Node> newNode = make_shared<Node>(args...);
                shared_ptr<Node> nextNode = node->next;

                if(!nextNode)
                    return nullptr;
                    
                nextNode->previous = newNode;
                newNode->next = nextNode;

                node->next = newNode;
                newNode->previous = node;

                return newNode;
            }
        }

        void Remove(shared_ptr<Node>& node){
            if(!node)
                return;

            if(node == head){
                shared_ptr<Node> nextNode = node->next;

                if(nextNode)
                    nextNode->previous.reset();

                head = nextNode; 
            } else if(node == tail){
                shared_ptr<Node> previousNode = node->previous.lock();

                if(previousNode)
                    previousNode->next.reset(); 

                tail = previousNode; 
            } else {
                shared_ptr<Node> nextNode = node->next;
                shared_ptr<Node> previousNode = node->previous.lock();

                if(previousNode)
                    previousNode->next = nextNode;


                if(nextNode)
                    nextNode->previous = previousNode;
            }
        }

        void Remove(T data){
            shared_ptr<Node> currentNode = head;
            while(currentNode){
                if(*currentNode->data == data){
                    Remove(currentNode);
                    return;
                }
                currentNode = currentNode->next;
            }
        }

        void RemoveBefore(shared_ptr<Node> node){
            if(!node)
                return;

            shared_ptr<Node> previousNode = node->previous.lock();
            Remove(previousNode);
        }

        void RemoveAfter(shared_ptr<Node> node){
            if(!node)
                return;

            shared_ptr<Node> nextNode = node->next;
            Remove(nextNode);
        }

        shared_ptr<Node> Find(T data){
            shared_ptr<Node> currentNode = head;
            while(currentNode){
                if(*currentNode->data == data){
                    return currentNode;
                }
                currentNode = currentNode->next;
            }

            return nullptr;
        }

        bool Contains(T data){
            shared_ptr<Node> currentNode = head;
            while(currentNode){
                if(*currentNode->data == data){
                    return true;
                }
                currentNode = currentNode->next;
            }

            return false;
        }

        Iterator begin() { return Iterator(head); };
        Iterator end(){
            if(!tail)
                return Iterator(tail);

            return Iterator(tail->next);
        }
};
