#include "doublylinkedlist.h"
#include <iostream>
#include <memory>
#include <string>

int main(){
    DoublyLinkedList<string> list;

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
}
