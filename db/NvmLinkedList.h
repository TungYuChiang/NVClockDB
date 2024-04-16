#ifndef NVM_LINKED_LIST_H
#define NVM_LINKED_LIST_H

#include <iostream>
#include <cstring>
#include "util/pm_manager.h"  

namespace leveldb {

class NvmNode {
public:
    char* key;
    char* data;
    size_t size;
    NvmNode* prev;
    NvmNode* next;

    struct Attributes {
        unsigned int status : 2;
        unsigned int twiceRead : 1;
    } attributes;

    enum NvmNodeStatus {
        Initial = 0,
        Be_Written = 1,
        Pre_Migration = 2,
        Migration = 3
    };

    NvmNode(char* key, char* data, size_t size): prev(nullptr), next(nullptr), size(size) {
        this->key = key;
        this->data = data;
        setStatus(Initial);   
        attributes.twiceRead = 0;
    }

    void setStatus(NvmNodeStatus status) {
        attributes.status = static_cast<unsigned int>(status);
    }

    NvmNodeStatus getStatus() const {
        return static_cast<NvmNodeStatus>(attributes.status);
    }

    bool isStatus(NvmNodeStatus status) const {
        return getStatus() == status;
    }
};

class NvmLinkedList {
public:
    NvmNode* head;
    NvmNode* tail;
    PMmanager* pm_;
    size_t currentSize;

    NvmLinkedList(PMmanager* pm): head(nullptr), tail(nullptr), pm_(pm), currentSize(0) {}

    NvmNode* createNode(std::string key, std::string data) {
        size_t keySize = key.size() + 1;
        size_t dataSize = data.size() + 1;
        size_t totalSize = sizeof(NvmNode) + keySize + dataSize;

        void* ptr = pm_->Allocate(totalSize);

        char* keyPtr = reinterpret_cast<char*>(ptr) + sizeof(NvmNode);
        char* dataPtr = keyPtr + keySize;

        strcpy(keyPtr, key.c_str());
        strcpy(dataPtr, data.c_str());

        NvmNode* newNode = new (ptr) NvmNode(keyPtr, dataPtr, totalSize);
        return newNode;
    }

    void insertNode(std::string key, std::string data) {
        NvmNode* newNode = createNode(key, data);
        if (!head) {
            head = tail = newNode;  
        } else {
            newNode->next = head;  
            head->prev = newNode;
            head = newNode;
        }
        currentSize += newNode->size;
    }

    void deleteNode(NvmNode* node) {
        if (!node) return;

        if (node == head) {
            head = node->next;  
        }
        if (node == tail) {
            tail = node->prev; 
        }
        if (node->prev) {
            node->prev->next = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        }

        pm_->Free(node);
        currentSize -= node->size;
    }

    void moveToHead(NvmNode* node) {
        if (node == head || !node) return; 

        if (node->prev) node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;

        if (node == tail) tail = node->prev;

        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;
    }

    ~NvmLinkedList() {
        while (head != nullptr) {
            NvmNode* next = head->next;
            pm_->Free(head);
            head = next;
        }
        currentSize = 0;
    }
};

} // namespace leveldb

#endif // NVM_LINKED_LIST_H
