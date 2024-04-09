#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H
#include <iostream>
#include <cstdint>
#include <string>
#include <cstring>
#include "util/pm_manager.h"

namespace leveldb{

class NvmNode {
public:
    char* key;
    char* data;
    size_t size;
    NvmNode* prev;
    NvmNode* next;

    struct Attributes {
        unsigned int reference : 1; 
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
        attributes.reference = 0;
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

class NvmCircularLinkedList {
public:
    NvmNode* head;
    PMmanager* pm_;
    size_t currentSize;  //Current size of this LinkedList

    NvmCircularLinkedList(PMmanager* pm): head(nullptr), pm_(pm), currentSize(0) {}

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
        currentSize += newNode->size; 
    }

    void deleteNode(NvmNode* node) {
        if (node == node->next) {
            head = nullptr;
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            if (head == node) head = node->next;
        }
        currentSize -= node->size; 
        pm_->Free(node);
    }

    ~NvmCircularLinkedList() {
        while (head != nullptr && head != head->next) {
            deleteNode(head);
        }
        if (head) {
            pm_->Free(head);
            head = nullptr;
        }
        currentSize = 0; 
    }
};
}// namespace leveldb
#endif // CIRCULAR_LIST_H