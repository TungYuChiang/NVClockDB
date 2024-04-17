#include "db/SLRUCache.h"
#include <gtest/gtest.h>
#include <string>

namespace leveldb{

class SLRUCacheTest : public ::testing::Test {
protected:
    SLRUCache* cache;
    PMmanager* pm;

    void SetUp() override {
        pm = new PMmanager("SLRUCache_Test");  // Assuming PMmanager has a default constructor
        cache = new SLRUCache(pm, 1024, 2048);  // Assuming these are reasonable capacities for DRAM and NVM
    }

    void TearDown() override {
        delete cache;
        delete pm;
    }

};

// Test to ensure that basic put operation works
TEST_F(SLRUCacheTest, BasicPut) {
    string key = "key1";
    string value = "value1";
    cache->put(key, value);
    
    auto it = cache->dram_cacheMap.find(key);
    ASSERT_NE(it, cache->dram_cacheMap.end());
    EXPECT_EQ(string(it->second->data), value);
}

// Test the overflow from DRAM to NVM
TEST_F(SLRUCacheTest, DRAMOverflowToNVM) {
    // Manually fill DRAM to capacity
    size_t estimatedSizePerEntry = sizeof(DramNode) + sizeof("keyX") + sizeof("valueX"); // Adjust size calculation as needed
    size_t numEntries = cache->dramCapacity / estimatedSizePerEntry;

    for (int i = 0; i < numEntries; ++i) {
        cache->put("key" + std::to_string(i), "value" + std::to_string(i));
    }

    cache->put("overflow", "overflow_value");

    auto nvm_it = cache->nvm_cacheMap.find("key0");
    ASSERT_NE(nvm_it, cache->nvm_cacheMap.end());
    EXPECT_EQ(string(nvm_it->second->data), "value0");
}

// Test to verify that updating a value works
TEST_F(SLRUCacheTest, UpdateValue) {
    std::string key = "key1";
    std::string initialValue = "initial_value";
    std::string updatedValue = "updated_value";
    
    cache->put(key, initialValue);
    cache->put(key, updatedValue);
    
    auto it = cache->dram_cacheMap.find(key);
    ASSERT_NE(it, cache->dram_cacheMap.end());
    EXPECT_EQ(std::string(it->second->data), updatedValue);
}

// Test the eviction logic when both DRAM and NVM are full
TEST_F(SLRUCacheTest, FullEviction) {
    for (int i = 0; i < 100; ++i) {
        cache->put("key" + std::to_string(i), "value" + std::to_string(i));
    }

    for (int i = 100; i < 300; ++i) {
        cache->put("key" + std::to_string(i), "value" + std::to_string(i));
    }
    
    cache->put("new_overflow", "new_value");

    // Check that the first NVM key was evicted
    auto nvm_it = cache->nvm_cacheMap.find("key100");
    EXPECT_EQ(nvm_it, cache->nvm_cacheMap.end());
}

// Test to ensure that basic get operation works
TEST_F(SLRUCacheTest, BasicGet) {
    string key = "key1";
    string value = "value1";
    cache->put(key, value);

    string retrievedValue;
    bool found = cache->get(key, &retrievedValue);
    ASSERT_TRUE(found);
    EXPECT_EQ(retrievedValue, value);
}

// Test getting value from NVM
TEST_F(SLRUCacheTest, GetFromNVM) {
    //fill dram
    size_t estimatedSizePerEntry = sizeof(DramNode) + sizeof("keyX") + sizeof("valueX");
    size_t numEntries = cache->dramCapacity / estimatedSizePerEntry;
    for (int i = 0; i < numEntries + 1; ++i) {
        cache->put("key" + std::to_string(i), "value" + std::to_string(i));
    }

    string value;
    bool found = cache->get("key0", &value);  // Assuming "key0" was moved to NVM
    ASSERT_TRUE(found);
    EXPECT_EQ(value, "value0");

    auto itNvm = cache->nvm_cacheMap.find("key0");
    ASSERT_NE(itNvm, cache->nvm_cacheMap.end());
    EXPECT_EQ(string(itNvm->second->data), "value0");
}

// Test status and twiceRead updates on get from NVM
TEST_F(SLRUCacheTest, NVMGetStatusUpdate) {
    cache->nvm_list.insertNode("key1", "value1");
    cache->nvm_cacheMap["key1"] = cache->nvm_list.head;

    string value;
    cache->get("key1", &value);  // First get, should set twiceRead to 1
    cache->get("key1", &value);  // Second get, should increase status

    NvmNode* node = cache->nvm_cacheMap["key1"];
    ASSERT_EQ(node->attributes.twiceRead, 0);
    ASSERT_EQ(node->attributes.status, Read_Twice);  
}

// Test the migration trigger on get from NVM
TEST_F(SLRUCacheTest, NVMGetMigrationTrigger) {
    // Insert a node directly into NVM for testing
    cache->nvm_list.insertNode("key_migration", "value_migration");
    auto nvmNode = cache->nvm_list.head;
    nvmNode->attributes.status = Migration - 1;  
    nvmNode->attributes.twiceRead = 1;
    cache->nvm_cacheMap["key_migration"] = nvmNode;
    cache->dram_cacheMap.erase("key_migration");  

    string value;
    bool result = cache->get("key_migration", &value);  

    ASSERT_TRUE(result);
    auto itDram = cache->dram_cacheMap.find("key_migration");
    ASSERT_NE(itDram, cache->dram_cacheMap.end());  
    EXPECT_EQ(string(itDram->second->data), "value_migration"); 

    auto itNvm = cache->nvm_cacheMap.find("key_migration");
    ASSERT_EQ(itNvm, cache->nvm_cacheMap.end()); 
}





}//namespace leveldb