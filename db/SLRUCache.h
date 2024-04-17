#ifndef S_LRU_CACHE_H
#define S_LRU_CACHE_H

#include "db/DramLinkedList.h"
#include "db/NvmLinkedList.h"
#include <iostream>
#include <unordered_map>
#include <list>
#include <gtest/gtest.h>
#include <string>

using std::string;
using std::unordered_map;

namespace leveldb {

class SLRUCache
{
private:
    friend class SLRUCacheTest;
    PMmanager *pm;
    DramLinkedList dram_list;
    NvmLinkedList nvm_list;
    unordered_map<string, DramNode*> dram_cacheMap;
    unordered_map<string, NvmNode*> nvm_cacheMap;
    size_t dramCapacity;
    size_t nvmCapacity;
public:
    SLRUCache(PMmanager *pm, size_t dramSize, size_t nvmSize);
    ~SLRUCache();
    void put(const string& key, const string& value);
    bool get(const string& key, string* value);
    void swapWithDram(NvmNode* node);
    bool deleteKey(const string& key);
    bool lookup(const string& key);

    //Testing function
    FRIEND_TEST(SLRUCacheTest, BasicPut);
    FRIEND_TEST(SLRUCacheTest, DRAMOverflowToNVM);
    FRIEND_TEST(SLRUCacheTest, UpdateValue);
    FRIEND_TEST(SLRUCacheTest, FullEviction);
    FRIEND_TEST(SLRUCacheTest, GetFromNVM);
    FRIEND_TEST(SLRUCacheTest, NVMGetStatusUpdate);
    FRIEND_TEST(SLRUCacheTest, NVMGetMigrationTrigger);
};

}// namespace leveldb 

#endif //S_LRU_CACHE_H