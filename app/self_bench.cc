#include "leveldb/db.h"
#include <cassert>
#include <iostream>
#include <libpmemobj.h>
#include <chrono>

using namespace std;
using namespace leveldb;
using namespace std::chrono;

int main() {
    //Database Setting
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    options.write_buffer_size = 64 * 1024 * 1024;
    options.block_cache = nullptr;
    leveldb::Status status = leveldb::DB::Open(options, "PMDB_1KB_10GB_selfbench_1", &db);
    assert(status.ok());
    if (!status.ok()) {
        std::cout<<"--------- db open error ----------- "<<std::endl;
    }
    cout << "Here is new data of PMDB"<<endl;
    cout<<endl;
    cout << "write_buffer_size: " << options.write_buffer_size / 1024 / 1024<< "MB" << endl;

    // Write Data Setting 
    int KB = 16;
    int entrySize = KB * 1024; //byte ,每个键值对的大约大小, 100byte
    double entryCount = 1L * 1024 * 1024 / KB; 

    // Write
    int count = 0;
    unsigned long long totalSize = 0;
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < entryCount; ++i) {
        /* count++;
        if (count % 100000 == 0) {
            std::cout<<"Finished "<<count / 10000<< "萬 operation"<<std::endl;
        }  */
        string key = "Key" + std::to_string(i);
        string value = string(entrySize - key.size(), 'v'); // 调整值的大小以使总大小接近100字节
        totalSize += key.size() + value.size();
        //cout<<key<<endl;
        db->Put(WriteOptions(), key, value);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout<<"寫入總量: "<<totalSize /(1024 * 1024)<<"MB"<<endl;
    cout<<"entrySize : "<<entrySize / 1024 <<"K byte"<<endl;
    cout << "Put operation took " << duration.count() << " milliseconds." << endl;
    cout << "Put operation took " << (duration.count() * 1000)/ entryCount<< " micro/ops" << endl;
    cout << "Write operation throughput (capacity): " << (totalSize / 1024.0 / 1024.0) / (duration.count() / 1000.0) << " MB/s" << endl;

    start = high_resolution_clock::now();
    int attemptedReads = 0;
    int successfulReads = 0;
    size_t totalReadSize = 0; // 成功读取的总字节数
    for (size_t i = 0; i < entryCount; ++i) {
        string key;
        if (i % 5 == 0) {
            key = "NonExistentKey" + std::to_string(i);
        } else {
            key = "Key" + std::to_string(i);
        }
        
        string value;
        Status s = db->Get(ReadOptions(), key, &value);
        attemptedReads++;
        if (s.ok()) {
            successfulReads++;
            totalReadSize += key.size() + value.size();
        }
    }
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Get operation (with 20% non-existent keys) took " << duration.count() << " milliseconds." << endl;
    cout << "Get operation (with 20% non-existent keys) took " << (duration.count() * 1000)/ entryCount<< " micro/ops" << endl;
    cout << "Read operation throughput (including non-existent keys): " << attemptedReads / (duration.count() / 1000.0) << " ops/sec" << endl;
    cout << "Read operation throughput (capacity): " << (totalReadSize / 1024.0 / 1024.0) / (duration.count() / 1000.0) << " MB/s" << endl;
    cout << "Attempted Reads: " << attemptedReads << ", Successful Reads: " << successfulReads << endl;
    cout << "Completed writing and reading " << entryCount << " entries." << endl;

    // 关闭数据库
    delete db;

    return 0;
}









