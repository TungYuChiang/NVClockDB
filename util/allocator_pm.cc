#include "allocator_pm.h"

namespace leveldb {
    static const size_t pmem_len = 1L * 1024 * 1024 * 1024;
    static const std::string path = "/home/oslab/Desktop/pmem/leveldb_allocator_test";

PMallocator::PMallocator(): used(0) {
    if ((pmemaddr = pmem_map_file(path.c_str(), pmem_len, PMEM_FILE_CREATE,
                                  0666, &mapped_len, &is_pmem)) == NULL) {
        perror("pmem_map_file error");
        exit(1);
    }
    free = pmem_len;
}

PMallocator::~PMallocator() {
    pmem_unmap(pmemaddr, mapped_len);
    pmemaddr = NULL;
}

void *PMallocator::Allocate(size_t bytes, int64_t &offset) {
    void *result = NULL;
    if (free < bytes) {
        return NULL;
    }
    if (free >= bytes) {
        free -= bytes;
        result = pmemaddr + used;
        offset = (int64_t)used;
        used += bytes;
    }
    return result;
}

//flush to PM
void PMallocator::Sync(void *start, size_t len) {
    if (is_pmem) {
        pmem_persist(start, len);
    } else {
        pmem_msync(start, len);
    }
}

void PMallocator::Recover(std::ifstream &ifs) {
    ifs >> used;
    ifs >> free;
}

void PMallocator::Save(std::ofstream &ofs) {
    ofs << used << std::endl;
    ofs << free << std::endl;
}

}   // leveldb