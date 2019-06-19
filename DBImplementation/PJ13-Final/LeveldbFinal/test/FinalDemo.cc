//
// Created by rui on 19-6-12.
//

#include <cassert>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/env.h"
#include "leveldb/table.h"
#include "db/filename.h"
#include "db/version_set.h"
#include "db/version_edit.h"
#include "db/dbformat.h"
#include <sys/time.h>
#include <ctime>

using namespace std;
using namespace leveldb;

string Trim(string &str) {
    str.erase(0, str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    return str;
}

void AddData(leveldb::DB *db) {
    ifstream fin("../test/TestData30w.csv");
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
        leveldb::Status status2 = db->Put(leveldb::WriteOptions(), key, value);
        assert(status2.ok());
    }
}


// 非主键过滤，使用布隆过滤器判断是否在该table中，返回该table的指针
bool isExistMail(leveldb::Options options, uint64_t tableNumber,
                 std::string strFind, uint64_t tableSize, leveldb::Table **Ptable) {

    std::string dbname_ = "../cmake-build-debug/mydb123";
    std::string fname = leveldb::TableFileName(dbname_, tableNumber);

    leveldb::RandomAccessFile *file = nullptr;
    leveldb::Table *table = nullptr;

    leveldb::Env *env_ = options.env;
    env_->NewRandomAccessFile(fname, &file);

    leveldb::Table::Open(options, file, tableSize, &table);

    *Ptable = table;

    // 构造slice
    uint64_t seq = 1;
    PutFixed64(&strFind, seq);

    return table->ValueInTable(Slice(strFind));
}


void addToMap(leveldb::Table *table, multimap<std::string, std::string> &mmap,
              vector<string> &ret) {

    leveldb::Iterator *result = table->NewIterator(leveldb::ReadOptions());

    result->SeekToFirst();
    while (result->Valid()) {
        leveldb::ParsedInternalKey ikey;
        leveldb::ParseInternalKey(result->key(), &ikey);
//        if (strcmp(result->value().ToString().c_str(), "NCC@ecnu.cn") == 0)
//            ret.push_back(ikey.user_key.ToString());
//            cout << result->value().ToString() << " " << ikey.user_key.ToString() << endl;
        mmap.insert(std::make_pair(result->value().ToString(), ikey.user_key.ToString()));
        result->Next();
    }

    return;
}


int main() {

    int id_min = 1;
    int id_max = 200000;
    std::string strFind = "TQM@ecnu.cn";

    // 1.initialize
    // 实现mycomparator

    // open a db
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);
    std::string dbname_ = "../cmake-build-debug/mydb123";
    leveldb::Status status2 = leveldb::DB::Open(options, dbname_, &db);

    // clock start
    timeval start, end;
    gettimeofday(&start, NULL);

    // put datas into sstables
//        AddData(db);

    // 构造multimap，用于模拟join操作
    multimap<std::string, std::string> mmap;
    vector<string> ret;


    // 找到每个ssts的最大最小值-通过最大最小值来减少ssts
    // 同时得到当前可能的table，用布隆过滤器判断是否可能有选定的mail
    // 如果有，则加入multimap

    leveldb::Version *current = db->GetCurrentVersion();

    for (int level = 0; level < current->GetLevelNums(); level++) {
        // get level i
        std::vector<leveldb::FileMetaData *> files_ = current->GetFiles(level);
//        if (files_.size() > 0) {
        // search level i tables
        for (int i = 0; i < files_.size(); i++) {

            int min_key = std::stoi(files_[i]->smallest.user_key().ToString());
            int max_key = std::stoi(files_[i]->largest.user_key().ToString());
//            cout<<"level "<<level<<endl;
//            cout << "min_key " << min_key << endl;
//            cout << "max_key " << max_key << endl;
            uint64_t tableNum = files_[i]->number;

            if (id_min > max_key || id_max < min_key) {
                continue;
            } else {
                // 该table id与 query重合，进行非主键过滤
                leveldb::Table *P_table = nullptr;
                uint64_t tableSize = files_[i]->file_size;
                if (isExistMail(options, tableNum, strFind, tableSize, &P_table)) {
                    addToMap(P_table, mmap, ret);
                }
            }

        }
//        } else
//            break;
    }

//    std::multimap<std::string, std::string>::iterator it = mmap.begin();
//    while (it != mmap.end()) {
//        std::cout << it->first << " " << it->second << std::endl;
//        it++;
//    }

//    vector<string>::iterator it = ret.begin();
//    string retString = "";
//    while (it != ret.end()) {
//        retString += (*it + " ");
//        it++;
//    }
//    cout << retString << strFind << endl;


    std::multimap<std::string, std::string>::iterator begIter = mmap.lower_bound(strFind);
    std::multimap<std::string, std::string>::iterator endIter = mmap.upper_bound(strFind);

    gettimeofday(&end, NULL);
//    cout << end.tv_usec<<endl;
    while (begIter != endIter)
        std::cout << strFind << " " << begIter++->second << std::endl;

//    cout << end.tv_usec<<endl;
//    cout << start.tv_usec<<endl;

    cout << end.tv_sec - start.tv_sec << "s," << end.tv_usec - start.tv_usec << "us" << endl;
    delete db;
    return 0;

}
