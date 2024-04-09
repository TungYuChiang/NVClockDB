#include "leveldb/db.h"
#include <cassert>
#include <iostream>
#include <libpmemobj.h>
#include <chrono>

using namespace std;
using namespace leveldb;
using namespace std::chrono;

int main() {
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "selfbench_3", &db);
    assert(status.ok());
    if (!status.ok()) {
        std::cout<<"--------- db open error ----------- "<<std::endl;
    }
    

    // 配置
    int entrySize = 100; //byte ,每个键值对的大约大小, 100byte
    double entryCount = 1 * 1000 * 1000; //

    // 写入数据
    int count = 0;
    int totalSize = 0;
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < entryCount; ++i) {
        /* count++;
        if (count % 100000 == 0) {
            std::cout<<"Finished "<<count / 10000<< "萬 operation"<<std::endl;
        } */
        string key = "Key" + std::to_string(i);
        string value = string(entrySize - key.size(), 'v'); // 调整值的大小以使总大小接近100字节
        totalSize += key.size() + value.size();
        //cout<<key<<endl;
        db->Put(WriteOptions(), key, value);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout<<"寫入總量: "<<totalSize /(1024 * 1024)<<"MB"<<endl;
    cout << "Put operation took " << duration.count() << " milliseconds." << endl;
    cout << "Put operation took " << (duration.count() * 1000)/ entryCount<< " micro/ops" << endl;

    // 读取数据
    start = high_resolution_clock::now();
    string key;
    for (size_t i = 0; i < entryCount / 2; ++i) {
        if (i % 5 == 0) {
            // 生成一个肯定不存在的键
            key = "NonExistentKey" + std::to_string(i);
        } else {
            key = "Key" + std::to_string(i);
        }
        string value;
        Status s = db->Get(ReadOptions(), key, &value);
        assert(s.ok());
    }
    for (size_t i = entryCount / 2; i > 1 ; i--) {
        if (i % 5 == 0) {
            // 生成一个肯定不存在的键
            key = "NonExistentKey" + std::to_string(i);
        } else {
            key = "Key" + std::to_string(i);
        }
        //cout<<key<<endl;
        string value;
        Status s = db->Get(ReadOptions(), key, &value);
        assert(s.ok());
    }
    end = high_resolution_clock::now();
    cout << "get operation took " << duration.count() << " milliseconds." << endl;
    cout << "get operation took " << (duration.count() * 1000)/ entryCount<< " micro/ops" << endl;
    cout << "Completed writing and reading " << entryCount << " entries." << endl;
    cout<<"寫入總量: "<<totalSize /(1024 * 1024)<<"MB"<<endl;

    // 关闭数据库
    delete db;

    return 0;
}









