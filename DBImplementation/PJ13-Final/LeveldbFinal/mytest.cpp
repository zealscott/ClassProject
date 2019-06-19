#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "db/version_set.h"
#include "db/version_edit.h"

using namespace std;
using namespace leveldb;

string Trim(string &str) {
    str.erase(0, str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    return str;
}


int main() {
    //opening a database
    leveldb::DB *db2;
    leveldb::Options options2;
    options2.create_if_missing = true;
    //options2.compression = kNoCompression;
    leveldb::Status status2 = leveldb::DB::Open(options2,
                                                "mydb123",
                                                &db2);
    assert(status2.ok());


    ifstream fin("../test/TestData.csv");
    string line;
    while (getline(fin, line)) {

        istringstream sin(line);
        vector<string> fields;
        string field;
        while (getline(sin, field, ',')) {
            fields.push_back(field);
        }
        string key = Trim(fields[0]);
        string value = Trim(fields[1]);
//        status2 =db2->Put(leveldb::WriteOptions(),key,value);
        assert(status2.ok());
    }


    // find level min-max
    leveldb::Version *current = db2->GetCurrentVersion();
    std::vector<leveldb::FileMetaData *> files_ = current->GetFiles(1);
    for (int i = 0; i < files_.size(); i++) {
        std::cout << "number: " << files_[i]->number << " ";
        std::cout << "id_min: " << files_[i]->smallest.user_key().ToString() << " ";
        std::cout << "id_max: " << files_[i]->largest.user_key().ToString() << std::endl;
    }

    delete db2;

    return 0;
}
