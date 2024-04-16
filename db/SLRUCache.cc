#include "db/SLRUCache.h"

namespace leveldb{

SLRUCache::SLRUCache(PMmanager* pm, size_t dramSize, size_t nvmSize) 
    : pm(pm), dramCapacity(dramSize), nvmCapacity(nvmSize), nvm_list(pm) {}

SLRUCache::~SLRUCache(){};

void SLRUCache::put(const string& key, const string& value) {
    size_t entrySize = sizeof(DramNode) + key.length() + 1 + value.length() + 1;

    if (entrySize > dramCapacity || entrySize > nvmCapacity) {
        std::cerr << "Entry size exceeds the capacity of DRAM or NVM." << std::endl;
        return;
    }

    if (auto it = dram_cacheMap.find(key); it != dram_cacheMap.end()) {
        if (value.length() <= strlen(it->second->data)) {
            strcpy(it->second->data, value.c_str());
            memset(it->second->data + value.length(), 0, strlen(it->second->data) - value.length());
            dram_list.moveToHead(it->second);
            return;
        } else {
            dram_list.deleteNode(it->second);
            dram_cacheMap.erase(it);
        }
    }

    while (dram_list.currentSize + entrySize > dramCapacity) {
        while (nvm_list.currentSize + entrySize > nvmCapacity) {
            nvm_cacheMap.erase(nvm_list.tail->key);
            nvm_list.deleteNode(nvm_list.tail);
        }

        dram_cacheMap.erase(dram_list.tail->key);

        nvm_list.insertNode(dram_list.tail->key, dram_list.tail->data);
        nvm_cacheMap[dram_list.tail->key] = nvm_list.head;

        dram_list.deleteNode(dram_list.tail);
    }

    // Insert new node into DRAM
    dram_list.insertNode(key.c_str(), value.c_str());
    dram_cacheMap[key] = dram_list.head;
}

} //namespace leveldb
