//
//  linked_list.hpp
//  src.common
//
//  Created by 杨丰 on 2020/11/4.
//

#ifndef linked_list_hpp
#define linked_list_hpp

#include <memory>

template <class T>
class LinkedList {
public:
    /// Linked List's Node Class Declaration
    class LinkedListNode {
    public:
        T value;
        std::shared_ptr<LinkedListNode> next;
        std::weak_ptr<LinkedListNode> previous;
        
        LinkedListNode(T value): value(value) {}
    };
    /// Typealiasing the node class to increase readability of code
    using Node = LinkedListNode;
    
public:
    /// Computed property to iterate through the linked list
    /// and return the last node in the list (if any)
    std::shared_ptr<Node> last() {
        if (head == nullptr) {
            return nullptr;
        }
        
        auto node = head;
        while (node->next != nullptr) {
            node = node->next;
        }
        return node;
    }
    
    std::shared_ptr<Node> first() {
        return head;
    }
    
    /// Computed property to check if the linked list is empty
    bool isEmpty() {
        return head == nullptr;
    }
    
    /// Computed property to iterate through the linked list
    /// and return the total number of nodes
    size_t count() {
        if (head == nullptr) {
            return 0;
        }
        
        size_t ct = 1;
        auto node = head;
        while (node->next != nullptr) {
            node = node->next;
            ct += 1;
        }
        return ct;
    }
    
    void append(T value) {
        auto newNode = std::make_shared<Node>(value);
        append(newNode);
    }
    
    void append(std::shared_ptr<Node> node) {
        auto newNode = node;
        auto lastNode = last();
        if (lastNode != nullptr) {
            newNode->previous = lastNode;
            lastNode->next = newNode;
        } else {
            head = newNode;
        }
    }
    
private:
    /// The head of the Linked List
    std::shared_ptr<Node> head;
};

#endif /* linked_list_hpp */
