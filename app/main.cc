#include "leveldb/db.h"
#include <cassert>
#include <iostream>
#include <libpmemobj.h>

using namespace std;
using namespace leveldb;

#define LAYOUT_NAME "rweg"
#define MAX_BUF_LEN 31

struct my_root {
	size_t len;
	char buf[MAX_BUF_LEN];
};

int main() {
    std::cout<<"----------- main ----------- "<<std::endl;
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "20240226", &db);
    //leveldb::Status status1 = leveldb::DB::Open(options, "now", &db);
    //leveldb::Status status = leveldb::DB::Open(options, "test1/db1", &db);
    
    assert(status.ok());
    if (!status.ok()) {
        std::cout<<"--------- db open error ----------- "<<std::endl;
    }
    status = db->Put(WriteOptions(), "Hellow", "Here is LevelDB");
    assert(status.ok());
    status = db->Put(WriteOptions(), "Test", "Test TwoLevelCache");
    assert(status.ok());
	string hwllowrod;
	status = db->Get(ReadOptions(), "Test", &hwllowrod);
    assert(status.ok());
	cout<<hwllowrod<<endl;

    delete db;
    
    cout<<"--------end main--------"<<endl;
}









