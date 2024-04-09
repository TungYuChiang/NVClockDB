#ifndef DRAM_CIRCULAR_LIST_DRAM_H
#define DRAM_CIRCULAR_LIST_DRAM_H

#include "DramCircularList.h"
#include <gtest/gtest.h>

namespace leveldb {

class CircularListDramTest : public ::testing::Test {
protected:
    DramCircularLinkedList* list;

    CircularListDramTest() {
    }

    virtual ~CircularListDramTest() {
    }

    virtual void SetUp() {
        list = new DramCircularLinkedList();
    }

    virtual void TearDown() {
        delete list;
    }
};

// 测试空链表的创建
TEST_F(CircularListDramTest, CreateEmptyList) {
    EXPECT_EQ(list->head, nullptr);
}

// 测试插入单个节点
TEST_F(CircularListDramTest, InsertSingleNode) {
    list->insertNode("key1", "data1");
    EXPECT_NE(list->head, nullptr);
    EXPECT_EQ(list->head->prev, list->head);
    EXPECT_EQ(list->head->next, list->head);
}

// 测试插入多个节点
TEST_F(CircularListDramTest, InsertMultipleNodes) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    EXPECT_NE(list->head, nullptr);
    EXPECT_NE(list->head->next, list->head);
    EXPECT_NE(list->head->prev, list->head);
    EXPECT_EQ(list->head->prev->next, list->head);
    EXPECT_EQ(list->head->next->prev, list->head);
}

// 测试删除节点
TEST_F(CircularListDramTest, DeleteNode) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->deleteNode(list->head->next); // 删除第二个插入的节点
    EXPECT_EQ(list->head->next, list->head);
    EXPECT_EQ(list->head->prev, list->head);
}

TEST_F(CircularListDramTest, TestLoop) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");
    
    DramNode* start = list->head;
    DramNode* iter = start->next;

    // 正向遍历
    bool loopedForward = false;
    while (iter != start) {
        iter = iter->next;
        if (iter == start) {
            loopedForward = true;
            break;
        }
    }
    EXPECT_TRUE(loopedForward);

    // 反向遍历
    iter = start->prev;
    bool loopedBackward = false;
    while (iter != start) {
        iter = iter->prev;
        if (iter == start) {
            loopedBackward = true;
            break;
        }
    }
    EXPECT_TRUE(loopedBackward);
}

TEST_F(CircularListDramTest, ClearList) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");

    // 逐个删除所有节点
    while (list->head != nullptr && list->head->next != list->head) {
        list->deleteNode(list->head);
    }
    // 最后删除剩余的最后一个节点
    list->deleteNode(list->head);

    // 验证链表是否为空
    EXPECT_EQ(list->head, nullptr);
}

//test enum
TEST_F(CircularListDramTest, NodeStatusTransition) {
    list->insertNode("key", "data");
    DramNode* node = list->head;

    node->setStatus(DramNode::Once_read);
    EXPECT_EQ(node->getStatus(), DramNode::Once_read);

    node->setStatus(DramNode::Twice_read);
    EXPECT_EQ(node->getStatus(), DramNode::Twice_read);

    node->setStatus(DramNode::Twice_read);
    EXPECT_EQ(node->getStatus(), DramNode::Twice_read);
}

// 测试插入节点后currentSize的更新
TEST_F(CircularListDramTest, CurrentSizeAfterInsertion) {
    list->insertNode("key1", "data1");
    size_t expectedSize = sizeof(DramNode) + strlen("key1") + 1 + strlen("data1") + 1;
    EXPECT_EQ(list->currentSize, expectedSize);

    list->insertNode("key2", "data2");
    expectedSize += sizeof(DramNode) + strlen("key2") + 1 + strlen("data2") + 1;
    EXPECT_EQ(list->currentSize, expectedSize);
}

// 测试删除节点后currentSize的更新
TEST_F(CircularListDramTest, CurrentSizeAfterDeletion) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    size_t initialSize = list->currentSize;

    list->deleteNode(list->head->next); // 删除"key2", "data2"
    size_t expectedSizeAfterDeletion = initialSize - (sizeof(DramNode) + strlen("key2") + 1 + strlen("data2") + 1);
    EXPECT_EQ(list->currentSize, expectedSizeAfterDeletion);
}

// 测试清空链表后currentSize是否为0
TEST_F(CircularListDramTest, CurrentSizeAfterClearingList) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");

    // 逐个删除所有节点
    while (list->head != nullptr && list->head->next != list->head) {
        list->deleteNode(list->head);
    }
    // 最后删除剩余的最后一个节点
    list->deleteNode(list->head);

    // 验证链表已清空且currentSize为0
    EXPECT_EQ(list->head, nullptr);
    EXPECT_EQ(list->currentSize, 0);
}

}//namespace leveldb 

#endif //DRAM_CIRCULAR_LIST_DRAM_H