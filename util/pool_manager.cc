#include "pool_manager.h"

#define LAYOUT_NAME "pool_layout"

namespace leveldb {
  static const size_t pmem_len = 1L * 1024 * 1024 * 1024;
  static const std::string path = "/home/oslab/Desktop/pmem/leveldb_";

int count = 0;

PMmanager::PMmanager(std::string db_name): used(0) {
    count++;
    std::cout<<count<<std::endl;
    std::string pool_name = path + db_name;
    pool = pmemobj_open(pool_name.c_str(), LAYOUT_NAME);
    if (pool != NULL) {
        std::cout<<pool_name<<" pool open"<<std::endl;
    }
    if (pool == NULL) {
        perror("pmemobj_open error");

        pool = pmemobj_create(pool_name.c_str(), LAYOUT_NAME, pmem_len, 0666);
        if (pool == NULL) {
            std::cout<<" pool create fail"<<std::endl;
            perror("pmemobj_create error");
            exit(1);
        } 
    }
    free = pmem_len;
}

PMmanager::~PMmanager() {
    //pmemobj_close(pool);     //有問題要解決
}

void *PMmanager::Allocate(size_t bytes) {
    std::cout<<"PMmanager::Allocate"<<std::endl;
    PMEMoid oid;
    int ret = pmemobj_alloc(pool, &oid, bytes, 0, NULL, NULL);
    std::cout<<"ret"<<std::endl;
    if (ret == 0) {
        return pmemobj_direct(oid);
    } else {
        // 分配失败，返回NULL
        return NULL;
    }
}

//flush to PM
void PMmanager::Sync(void *start, size_t len) {
    if (is_pmem) {
        pmem_persist(start, len);
    } else {
        pmem_msync(start, len);
    }
}

}   // leveldb