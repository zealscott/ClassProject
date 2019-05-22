#pragma once
#include "global.h"
#include<queue>
#include<string>
#include<unordered_map>
#include<mutex>
#include<vector>

#include"data_to.h"
using namespace std;

/*for cc_lock*/
struct LockEntry {
	lock_t type;
	std::thread::id tid;
};

class Engine {
public:
	Engine() {
		cc = CC_LOCK;
	}
	Engine(cc_type type) {
		cc = type;
	}
	RC find();
	RC update();
	RC delete_();

	//std::vector<Data> data_;
	//std::unordered_map<std::string, Data> data_map;  //hash map£¬use for store data
	std::unordered_map<std::string, data_to> data_map;
private:
	cc_type cc;
};