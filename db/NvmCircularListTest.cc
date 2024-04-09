#include "NvmCircularList.h"
#include "util/pm_manager.h"
#include <gtest/gtest.h>
#include <string>

namespace leveldb{
class CircularListNvmTest : public ::testing::Test {
protected:

    const size_t capacity = 4 * 100; //400 byte
    PMmanager* pm;

    void SetUp() override {
        pm = new PMmanager("circular_list_gtest");
    }

    void TearDown() override {
        delete pm;
    }
};


TEST_F(CircularListNvmTest, InsertAndCheckIntegrity) {
    NvmCircularLinkedList list(pm);

    list.insertNode("key1", "data1");
    NvmNode* head = list.head;
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head, head->next); 
    EXPECT_EQ(head, head->prev); 
    EXPECT_STREQ(head->key, "key1");
    EXPECT_STREQ(head->data, "data1");

    list.insertNode("key2", "data2");
    EXPECT_EQ(head, head->prev->next); 
    EXPECT_EQ(head, head->next->prev); 
    EXPECT_STREQ(head->next->key, "key2");
    EXPECT_STREQ(head->next->data, "data2");

    // 确保能够形成闭环
    EXPECT_EQ(head->next->next, head);
    EXPECT_EQ(head->prev, head->next);
}

TEST_F(CircularListNvmTest, DeleteNodeUpdatesLinksCorrectly) {
    NvmCircularLinkedList list(pm);

    list.insertNode("key1", "data1");
    list.insertNode("key2", "data2");
    list.insertNode("key3", "data3");

    NvmNode* nodeToDelete = list.head->next; 
    list.deleteNode(nodeToDelete);

    EXPECT_STREQ(list.head->key, "key1");
    EXPECT_STREQ(list.head->next->key, "key3");
    EXPECT_EQ(list.head->next->next, list.head); 
    EXPECT_EQ(list.head->prev, list.head->next); 
}

TEST_F(CircularListNvmTest, DeleteLastNode) {
    NvmCircularLinkedList list(pm);

    list.insertNode("key1", "data1");
    NvmNode* nodeToDelete = list.head;
    list.deleteNode(nodeToDelete);

    EXPECT_EQ(list.head, nullptr); 
}

TEST_F(CircularListNvmTest, MultipleInsertionsAndDeletions) {
    NvmCircularLinkedList list(pm);

    for (int i = 1; i <= 5; ++i) {
        list.insertNode("key" + std::to_string(i), "data" + std::to_string(i));
    }

    list.deleteNode(list.head->next->next); 

    NvmNode* current = list.head;
    int count = 0;
    do {
        EXPECT_NE(current, nullptr);
        current = current->next;
        ++count;
    } while (current != list.head);

    EXPECT_EQ(count, 4);

    while (list.head != nullptr) {
        list.deleteNode(list.head);
    }

    EXPECT_EQ(list.head, nullptr); 
}
TEST_F(CircularListNvmTest, DeleteFromSingleItemList) {
    NvmCircularLinkedList list(pm);

    list.insertNode("singleKey", "singleData");
    NvmNode* nodeToDelete = list.head;
    list.deleteNode(nodeToDelete);

    EXPECT_EQ(list.head, nullptr);
}

//測試enum
TEST_F(CircularListNvmTest, NodeStatusTransition) {
    NvmCircularLinkedList list(pm);
    list.insertNode("key", "data");
    NvmNode* node = list.head;

    node->setStatus(NvmNode::Be_Written);
    EXPECT_EQ(node->getStatus(), NvmNode::Be_Written);

    node->setStatus(NvmNode::Pre_Migration);
    EXPECT_EQ(node->getStatus(), NvmNode::Pre_Migration);

    node->setStatus(NvmNode::Migration);
    EXPECT_EQ(node->getStatus(), NvmNode::Migration);
}
// 测试插入节点后currentSize的更新
TEST_F(CircularListNvmTest, CurrentSizeAfterInsertion) {
    NvmCircularLinkedList list(pm);

    list.insertNode("key1", "data1");
    size_t expectedSizeAfterFirstInsert = sizeof(NvmNode) + strlen("key1") + 1 + strlen("data1") + 1;
    EXPECT_EQ(list.currentSize, expectedSizeAfterFirstInsert);

    list.insertNode("key2", "data2");
    size_t expectedSizeAfterSecondInsert = expectedSizeAfterFirstInsert + sizeof(NvmNode) + strlen("key2") + 1 + strlen("data2") + 1;
    EXPECT_EQ(list.currentSize, expectedSizeAfterSecondInsert);
}

// 测试删除节点后currentSize的更新
TEST_F(CircularListNvmTest, CurrentSizeAfterDeletion) {
    NvmCircularLinkedList list(pm);

    list.insertNode("key1", "data1");
    list.insertNode("key2", "data2");
    size_t sizeBeforeDeletion = list.currentSize;

    NvmNode* nodeToDelete = list.head->next; // 删除第二个插入的节点
    list.deleteNode(nodeToDelete);
    size_t expectedSizeAfterDeletion = sizeBeforeDeletion - (sizeof(NvmNode) + strlen("key2") + 1 + strlen("data2") + 1);
    EXPECT_EQ(list.currentSize, expectedSizeAfterDeletion);
}

// 测试清空链表后currentSize是否为0
TEST_F(CircularListNvmTest, CurrentSizeAfterClearingList) {
    NvmCircularLinkedList list(pm);

    list.insertNode("key1", "data1");
    list.insertNode("key2", "data2");
    list.insertNode("key3", "data3");

    while (list.head != nullptr && list.head->next != list.head) {
        list.deleteNode(list.head);
    }

    list.deleteNode(list.head);

    EXPECT_EQ(list.head, nullptr);
    EXPECT_EQ(list.currentSize, 0);
}

}//leveldb namespace