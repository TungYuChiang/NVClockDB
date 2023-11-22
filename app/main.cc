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
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "db_test_pm", &db);
    assert(status.ok());
	cout<<"1st key value"<<endl;
    //status = db->Put(WriteOptions(), "Hellow", "Here is LevelDB");
    //assert(status.ok());
	string res, hwllowrod;
	status = db->Get(ReadOptions(), "Hellow", &hwllowrod);
    assert(status.ok());
	cout<<hwllowrod<<endl;
    delete db;
}








//----------------------------------------test pmdk-----------------------------------
	/*
    const char* pmem_file = "/home/oslab/Desktop/pmem/levedb_test";
	PMEMobjpool *pop = pmemobj_create(pmem_file, 
		LAYOUT_NAME, PMEMOBJ_MIN_POOL, 0666);

	if (pop == NULL) {
		perror("pmemobj_create");
		exit(1);
	}

	PMEMoid root = pmemobj_root(pop, 
		sizeof(struct my_root));

	struct my_root *rootp = (my_root*)pmemobj_direct(root);

	char buf[MAX_BUF_LEN] = "Hello PMEM World";

	rootp->len = strlen(buf);
	pmemobj_persist(pop, &rootp->len, 
		sizeof(rootp->len));

	pmemobj_memcpy_persist(pop, rootp->buf, buf, 
		rootp->len);

	pmemobj_close(pop);

	exit(0);
    return 0;
	*/
