#ifndef DRAM_LINKED_LIST_H
#define DRAM_LINKED_LIST_H

#include <iostream>
#include <cstring>
#include <gtest/gtest.h>

namespace leveldb {

enum DramNodeStatus {
    Initial = 0,
    Read_Level1 = 1,
    Be_Migration = 2,
    Protected = 3
};

class DramNode {
public:
    char* key;
    char* data;
    size_t size; 
    DramNode* next;
    DramNode* prev;

    struct Attributes {
        unsigned int status : 2;
    } attributes;

    DramNode(const char* key, const char* data) {
        this->key = strdup(key);
        this->data = strdup(data);
        setStatus(Initial); 
        this->size = sizeof(DramNode) + strlen(key) + strlen(data) + 2;
        this->next = nullptr;
        this->prev = nullptr;
    }

    ~DramNode() {
        free(key);
        free(data);
    }

    void setStatus(DramNodeStatus status) {
        attributes.status = static_cast<unsigned int>(status);
    }

    DramNodeStatus getStatus() const {
        return static_cast<DramNodeStatus>(attributes.status);
    }

    bool isStatus(DramNodeStatus status) const {
        return getStatus() == status;
    }
};

class DramLinkedList {
public:
    DramNode* head;
    DramNode* tail;
    size_t currentSize;

    DramLinkedList() : head(nullptr), tail(nullptr), currentSize(0) {}

    void insertNode(const char* key, const char* data) {
        DramNode* newNode = new DramNode(key, data);
        if (!head) {
            head = tail = newNode; 
        } else {
            newNode->next = head;  
            head->prev = newNode;  
            head = newNode;        
        }
        currentSize += newNode->size;
    }

    void deleteNode(DramNode* node) {
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
        currentSize -= node->size;
        delete node;
    }

    void moveToHead(DramNode* node) {
        if (!node || node == head) return;

        if (node->prev) {
            node->prev->next = node->next;
        }
        if (node->next) {
            node->next->prev = node->prev;
        }
        node->next = head;
        node->prev = nullptr;
        if (head) {
            head->prev = node;
        }
        head = node;
        if (!tail) {
            tail = node;  
        }
    }

    ~DramLinkedList() {
        while (head) {
            deleteNode(head);
        }
    }
};

} // namespace leveldb

#endif // DRAM_LINKED_LIST_H
