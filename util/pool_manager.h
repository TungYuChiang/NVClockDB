#ifndef STORAGE_LEVELDB_UTIL_ALLOCATOR_PM_H
#define STORAGE_LEVELDB_UTIL_ALLOCATOR_PM_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <libpmem.h>
#include <libpmemobj.h>
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

namespace leveldb {

class PMmanager {
public:
    PMmanager();
    PMmanager(std::string pool_name);
    ~PMmanager();
    void Sync(void *start, size_t len);
    void* Allocate(size_t bytes);

private:
    size_t mapped_len;
    int is_pmem;
    size_t used;
    size_t free;
    PMEMobjpool *pool = NULL;
};

}   // leveldb
#endif // STORAGE_LEVELDB_UTIL_ALLOCATOR_PM_H