#ifndef DRAM_LINKED_LIST_TEST_H
#define DRAM_LINKED_LIST_TEST_H

#include "DramLinkedList.h"
#include <gtest/gtest.h>
#include <cstring>

namespace leveldb {

class DramLinkedListTest : public ::testing::Test {
protected:
    DramLinkedList* list;

    virtual void SetUp() override {
        list = new DramLinkedList();
    }

    virtual void TearDown() override {
        delete list;
    }
};

// 测试创建空链表
TEST_F(DramLinkedListTest, CreateEmptyList) {
    EXPECT_EQ(list->head, nullptr);
    EXPECT_EQ(list->currentSize, 0);
}

// 测试插入单个节点
TEST_F(DramLinkedListTest, InsertSingleNode) {
    list->insertNode("key1", "data1");
    EXPECT_NE(list->head, nullptr);
    EXPECT_EQ(list->head->prev, nullptr); // head->prev should be nullptr in a non-circular list
    EXPECT_EQ(list->head->next, nullptr); // head->next should be nullptr as there's only one node
    size_t expectedSize = sizeof(DramNode) + strlen("key1") + 1 + strlen("data1") + 1;
    EXPECT_EQ(list->currentSize, expectedSize);
}

// 测试插入多个节点
TEST_F(DramLinkedListTest, InsertMultipleNodes) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    EXPECT_NE(list->head, nullptr);
    EXPECT_EQ(list->tail->next, nullptr);  
    EXPECT_EQ(list->head->prev, nullptr);  
    EXPECT_NE(list->head, list->tail);     

    // Check the linkage is correct
    EXPECT_EQ(list->head->next, list->tail);  
    EXPECT_EQ(list->tail->prev, list->head);  

    size_t expectedSize = (sizeof(DramNode) + strlen("key1") + 1 + strlen("data1") + 1) +
                          (sizeof(DramNode) + strlen("key2") + 1 + strlen("data2") + 1);
    EXPECT_EQ(list->currentSize, expectedSize);
}


// 测试删除单个节点的情况，特别是当它是列表中唯一的节点时
TEST_F(DramLinkedListTest, DeleteSingleNode) {
    list->insertNode("key1", "data1");
    DramNode* nodeToDelete = list->head;
    list->deleteNode(nodeToDelete);

    EXPECT_EQ(list->head, nullptr);
    EXPECT_EQ(list->tail, nullptr);  // tail should also be nullptr
    EXPECT_EQ(list->currentSize, 0);
}

// 测试删除多个节点，包括头节点和尾节点
TEST_F(DramLinkedListTest, DeleteHeadAndTailNodes) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");

    // 删除头节点
    DramNode* headNodeToDelete = list->head;
    list->deleteNode(headNodeToDelete);
    EXPECT_NE(list->head, headNodeToDelete);
    EXPECT_STREQ(list->head->data, "data2");

    // 删除尾节点
    DramNode* tailNodeToDelete = list->tail;
    list->deleteNode(tailNodeToDelete);
    EXPECT_NE(list->tail, tailNodeToDelete);
    EXPECT_STREQ(list->tail->data, "data2");

    size_t expectedSize = sizeof(DramNode) + strlen("key2") + 1 + strlen("data2") + 1;
    EXPECT_EQ(list->currentSize, expectedSize);
}

// 测试删除中间的节点
TEST_F(DramLinkedListTest, DeleteMiddleNode) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");

    DramNode* middleNodeToDelete = list->head->next;
    list->deleteNode(middleNodeToDelete);
    EXPECT_EQ(list->head->next, list->tail);
    EXPECT_EQ(list->tail->prev, list->head);

    size_t expectedSize = (sizeof(DramNode) + strlen("key1") + 1 + strlen("data1") + 1) +
                          (sizeof(DramNode) + strlen("key3") + 1 + strlen("data3") + 1);
    EXPECT_EQ(list->currentSize, expectedSize);
}

// 测试清空链表
TEST_F(DramLinkedListTest, ClearList) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");

    while (list->head) {
        list->deleteNode(list->head);
    }

    EXPECT_EQ(list->head, nullptr);
    EXPECT_EQ(list->tail, nullptr);  
    EXPECT_EQ(list->currentSize, 0);
}

// 测试 moveToHead 功能
TEST_F(DramLinkedListTest, MoveNodeToHead) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");

    ASSERT_NE(list->head->key, "key2");

    DramNode* node = list->head;
    while (node && strcmp(node->key, "key2") != 0) {
        node = node->next;
    }

    ASSERT_NE(node, nullptr);

    list->moveToHead(node);

    EXPECT_STREQ(list->head->key, "key2");
}

} // namespace leveldb

#endif // DRAM_LINKED_LIST_TEST_H
