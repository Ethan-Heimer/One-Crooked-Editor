#include "doublylinkedlist.h"

#include <iostream>
#include <memory>
#include <string>

int main(){
    DoublyLinkedList<string> list;

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
    DoublyLinkedList<string> list2;

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
    std::shared_ptr<DoublyLinkedList<string>::Node> currentNode = t3;
    while(currentNode != nullptr){
        std::cout << *currentNode->data << std::endl;

        currentNode = currentNode->previous.lock();
    }

}
