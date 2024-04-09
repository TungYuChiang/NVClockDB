#ifndef CIRCULAR_LIST_DRAM_H
#define CIRCULAR_LIST_DRAM_H

#include <iostream>
#include <cstdint>
#include <string>
#include <cstring> 

using std::string;

namespace leveldb {

class DramNode {
public:
    char* key;
    char* data;
    size_t size;
    DramNode* prev;
    DramNode* next;
    struct {
        unsigned int reference : 1; 
        unsigned int status : 2;     
    } attributes;

    enum DramNodeStatus {
        Initial = 0,
        Once_read = 1,
        Twice_read = 2,
        Be_Migration = 3
    };

    void setStatus(DramNodeStatus status) {
        attributes.status = static_cast<unsigned int>(status);
    }

    DramNodeStatus getStatus() const {
        return static_cast<DramNodeStatus>(attributes.status);
    }


    DramNode(const std::string& key, const std::string& data, size_t size): prev(nullptr), next(nullptr), size(size){
        size_t keySize = key.size() + 1;
        size_t dataSize = data.size() + 1;
        this->key = new char[keySize];
        this->data = new char[dataSize];
        std::strcpy(this->key, key.c_str());
        std::strcpy(this->data, data.c_str());
        attributes.reference = 0; 
        attributes.status = 0; 
    }
};

class DramCircularLinkedList {
public:
    DramNode* head;
    size_t currentSize; // 当前链表占用的总大小

    DramCircularLinkedList(): head(nullptr), currentSize(0) {}

    //在DramNode()裡面會使用char來做搬移，實際大小會是key.size() + 1
    void insertNode(const std::string& key, const std::string& data) {
        size_t nodeSize = (key.size() + 1) + (data.size() + 1 )+ sizeof(DramNode);
        DramNode* newNode = new DramNode(key, data, nodeSize);
        if (head == nullptr) {
            head = newNode;
            newNode->next = newNode;
            newNode->prev = newNode;
        } else {
            newNode->next = head;
            newNode->prev = head->prev;
            head->prev->next = newNode;
            head->prev = newNode;
        }
        currentSize += nodeSize; 
    }

    void deleteNode(DramNode* node) {
        if (node == nullptr) return;

        if (node == node->next) {
            head = nullptr;
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            if (head == node) head = node->next;
        }
        currentSize -= node->size; 
        delete[] node->key;
        delete[] node->data;
        delete node;
    }

    ~DramCircularLinkedList() {
        while (head != nullptr && head != head->next) {
            deleteNode(head);
        }
        if (head) {
            delete head;
            head = nullptr;
        }
    }
};

}//namespace 

#endif // CIRCULAR_LIST_DRAM_H
