#include "db/NvmLinkedList.h"
#include "util/pm_manager.h"
#include <gtest/gtest.h>
#include <string>
#include <cstring>

namespace leveldb {
class NvmLinkedListTest : public ::testing::Test {
protected:
    const size_t capacity = 400;  // Assume 400 bytes for capacity
    PMmanager* pm;
    NvmLinkedList* list;

    void SetUp() override {
        pm = new PMmanager("linked_list_gtest");
        list = new NvmLinkedList(pm);
    }

    void TearDown() override {
        delete list;
        delete pm;
    }
};

TEST_F(NvmLinkedListTest, CreateEmptyList) {
    EXPECT_EQ(list->head, nullptr);
    EXPECT_EQ(list->tail, nullptr);
}

TEST_F(NvmLinkedListTest, InsertSingleNode) {
    list->insertNode("key1", "data1");
    EXPECT_NE(list->head, nullptr);
    EXPECT_EQ(list->head->prev, nullptr);  // head->prev should be nullptr in a non-circular list
    EXPECT_EQ(list->head->next, nullptr);  // head->next should be nullptr since there's only one node
    size_t expectedSize = sizeof(NvmNode) + strlen("key1") + 1 + strlen("data1") + 1;
    std::cout << "Size of NvmNode: " << sizeof(NvmNode) << " bytes" << std::endl;
    EXPECT_EQ(list->currentSize, expectedSize);
}

TEST_F(NvmLinkedListTest, InsertMultipleNodes) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    EXPECT_NE(list->head, nullptr);
    EXPECT_NE(list->head->next, nullptr);
    EXPECT_EQ(list->head->next, list->tail);  // head's next should point to tail
    EXPECT_EQ(list->tail->prev, list->head);  // tail's prev should point to head
    size_t expectedSize = (sizeof(NvmNode) + strlen("key1") + 1 + strlen("data1") + 1) +
                          (sizeof(NvmNode) + strlen("key2") + 1 + strlen("data2") + 1);
    EXPECT_EQ(list->currentSize, expectedSize);
}

TEST_F(NvmLinkedListTest, DeleteNodeUpdatesLinksCorrectly) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");

    EXPECT_STREQ(list->head->key, "key3");
    EXPECT_STREQ(list->head->next->key, "key2");
    EXPECT_STREQ(list->head->next->next->key, "key1");
    EXPECT_EQ(list->head->next->next, list->tail); 

    NvmNode* nodeToDelete = list->head->next;  
    list->deleteNode(nodeToDelete);

    EXPECT_EQ(list->head->next, list->tail);  
    EXPECT_EQ(list->tail->prev, list->head);  
    EXPECT_STREQ(list->tail->key, "key1");   
    EXPECT_EQ(list->tail->next, nullptr);     
    EXPECT_EQ(list->head->prev, nullptr);    
}


TEST_F(NvmLinkedListTest, DeleteHeadNode) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");

    list->deleteNode(list->head);  // Delete the first node

    EXPECT_EQ(strcmp(list->head->key, "key1"), 0);
    EXPECT_EQ(list->head->prev, nullptr);
}

TEST_F(NvmLinkedListTest, DeleteTailNode) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");

    list->deleteNode(list->tail);  // Delete the last node

    EXPECT_EQ(strcmp(list->tail->key, "key2"), 0);
    EXPECT_EQ(list->tail->next, nullptr);
}

TEST_F(NvmLinkedListTest, ClearList) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");

    while (list->head != nullptr) {
        list->deleteNode(list->head);
    }

    EXPECT_EQ(list->head, nullptr);
    EXPECT_EQ(list->tail, nullptr);
}

TEST_F(NvmLinkedListTest, CurrentSizeAfterInsertion) {
    list->insertNode("key1", "data1");
    size_t expectedSize = sizeof(NvmNode) + strlen("key1") + 1 + strlen("data1") + 1;
    EXPECT_EQ(list->currentSize, expectedSize);

    list->insertNode("key2", "data2");
    expectedSize += sizeof(NvmNode) + strlen("key2") + 1 + strlen("data2") + 1;
    EXPECT_EQ(list->currentSize, expectedSize);
}

TEST_F(NvmLinkedListTest, CurrentSizeAfterDeletion) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    size_t initialSize = list->currentSize;

    list->deleteNode(list->tail);  // Delete the last node
    size_t expectedSizeAfterDeletion = initialSize - (sizeof(NvmNode) + strlen("key2") + 1 + strlen("data2") + 1);
    EXPECT_EQ(list->currentSize, expectedSizeAfterDeletion);
}

TEST_F(NvmLinkedListTest, MoveToHead) {
    list->insertNode("key1", "data1");
    list->insertNode("key2", "data2");
    list->insertNode("key3", "data3");

    ASSERT_NE(list->head->key, "key2");

    NvmNode* node = list->head;
    while (node && strcmp(node->key, "key2") != 0) {
        node = node->next;
    }

    ASSERT_NE(node, nullptr);

    list->moveToHead(node);

    EXPECT_STREQ(list->head->key, "key2");
}

}  // namespace leveldb
