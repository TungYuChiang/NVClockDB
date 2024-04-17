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

bool SLRUCache::get(const string& key, string* value) {
    auto itDram = dram_cacheMap.find(key);
    if (itDram != dram_cacheMap.end()) {
        *value = itDram->second->data;  
        itDram->second->attributes.status++;  
        dram_list.moveToHead(itDram->second); 
        return true;
    }

    auto itNvm = nvm_cacheMap.find(key);
    if (itNvm != nvm_cacheMap.end()) {
        *value = itNvm->second->data;
        if (itNvm->second->attributes.twiceRead == 1) {
            itNvm->second->attributes.status++; 
            itNvm->second->attributes.twiceRead = 0;
        } else {
            itNvm->second->attributes.twiceRead = 1;  
        }
        nvm_list.moveToHead(itNvm->second);

        if (itNvm->second->attributes.status == Migration) {
            if ((dram_list.currentSize + itNvm->second->size) > dramCapacity) {
                swapWithDram(itNvm->second);
            } else { 
                //TODO: 多線程的問題
                dram_list.insertNode(itNvm->second->key, itNvm->second->data);
                dram_cacheMap[itNvm->second->key] = dram_list.head;  
                nvm_cacheMap.erase(itNvm);
                nvm_list.deleteNode(itNvm->second);
                dram_list.head->setStatus(Protected); 
            }
        }

        return true;
    }

    return false; //not found
}

void SLRUCache::swapWithDram(NvmNode* nvmNode) {
    if (nvmNode == nullptr || nvmNode->attributes.status != Migration) {
        return; 
    }

    if (nvmNode->size > dramCapacity) {
        //node size > dramCapacity, impossible to finish swap
        return;
    }

    DramNode* candidate = dram_list.tail;
    while (candidate != nullptr) {
        if (candidate->attributes.status == Protected) {
            candidate->attributes.status--;
        } else if (candidate->attributes.status <= Be_Migration) {
            break;
        }
        candidate = candidate->prev;  
    }

    if (candidate == nullptr) {
        return;
    }
    size_t newDramSize = dram_list.currentSize - candidate->size + nvmNode->size;
    size_t newNvmSize = nvm_list.currentSize - nvmNode->size + candidate->size;

    if (newDramSize > dramCapacity || newNvmSize > nvmCapacity) {
        // TODO: 超出上限的做法
        return;
    }

    // nvm node move to dram 
    dram_list.insertNode(nvmNode->key, nvmNode->data);
    dram_list.head->setStatus(Protected); 
    dram_cacheMap[nvmNode->key] = dram_list.head;
    nvm_cacheMap.erase(nvmNode->key);
    nvm_list.deleteNode(nvmNode);

    //dram node move to nvm node 
    nvm_list.insertNode(candidate->key, candidate->data);
    nvm_list.head->setStatus(NvmNodeStatus::NvmNodeInit);
    nvm_cacheMap[candidate->key] = nvm_list.head;
    dram_cacheMap.erase(candidate->key);
    dram_list.deleteNode(candidate);

    return;
}



} //namespace leveldb
