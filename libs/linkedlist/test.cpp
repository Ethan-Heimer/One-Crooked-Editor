#include "doublylinkedlist.h"

#include <iostream>
#include <memory>
#include <string>

int main(){
    DoublyIndexedLinkedList<string> list;

    std::cout << "Basic Appending and removal" << std::endl;
    auto node1 = list.Append("Hello");
    auto node2 = list.Append("World");

    list.AppendBefore(node1, "Hi!");
    list.AppendBefore(node2, "Hey");

    list.AppendAfter(node1, "Howdy!");
    list.AppendAfter(node2, "Yo");

    list.Append("Earth");

    for(auto node : list){
        std::cout << *node << endl;
    }

    std::cout << "Removing" << endl;

    list.Remove(node1);
    list.Remove("World");

    auto node3 = list.Find("Yo");

    list.RemoveBefore(node3);
    list.RemoveAfter(node3);

    for(auto node : list){
        std::cout << *node << endl;
    }

    std::cout << "Appeding - removal at the tail" << std::endl;
    DoublyIndexedLinkedList<string> list2;

    list2.Append("Head");
    list2.Append("Body");
    auto tail = list2.Append("Tail");

    auto t1 = list2.AppendAfter(tail, "Post Tail 1");
    auto t2 = list2.AppendAfter(t1, "Post Tail 2");
    auto t3 = list2.AppendAfter(t2, "Post Tail 3");

    for(auto node : list2){
        std::cout << *node << endl;
    }

    std::cout << "Backwards" << std::endl;
    std::shared_ptr<DoublyIndexedLinkedList<string>::Node> currentNode = t3;
    while(currentNode != nullptr){
        std::cout << *currentNode->data << std::endl;

        currentNode = currentNode->previous.lock();
    }

    std::cout << "Testing Indexes" << std::endl;

    DoublyIndexedLinkedList<string> list3;
    auto n1 = list3.Append("Hello");
    auto n2 = list3.Append("World");
    auto n3 = list3.Append("This is");
    auto n4 = list3.Append("a Test");

    for(auto node : list3){
        std::cout << node.index << " " << *node << std::endl;
    }

    std::cout << "Append before and after" << std::endl;

    list3.AppendAfter(n3, "AHHHHHH");
    list3.AppendBefore(n3, "BAHHHHHHH");

    for(auto node : list3){
        std::cout << node.index << " " << *node << std::endl;
    }

    std::cout << "Removal" << std::endl;

    list3.Remove(n1);
    list3.Remove(n4);
    list3.Remove(n3);

    for(auto node : list3){
        std::cout << node.index << " " << *node << std::endl;
    }
}
