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
    PMmanager(std::string pool_name);
    ~PMmanager();
    void Sync(void *start, size_t len);
    void* Allocate(size_t bytes);
    void Free(void* ptr);

private:
    size_t mapped_len;
    int is_pmem;
    size_t used;
    size_t free;
    PMEMobjpool *pool = NULL;
    bool create_directory(const std::string& path) {
        size_t pos = 0;
        std::string dir;
        int ret = 0;

        while ((pos = path.find_first_of('/', pos)) != std::string::npos){
            dir = path.substr(0, pos++);
            if (dir.size() == 0) continue; // 如果是以 '/' 开始的路径，跳过
            ret = mkdir(dir.c_str(), 0777); // 尝试创建目录
            if (ret != 0 && errno != EEXIST) return false; // 如果出错且不是因为目录已存在，则返回错误
        }
        return true; // 所有目录创建成功
    }
};

}   // leveldb
#endif // STORAGE_LEVELDB_UTIL_ALLOCATOR_PM_H