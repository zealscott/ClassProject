#pragma once
#include "global.h"
#include "structure.h"
#include<thread>
#include<ctime>
#include<mutex>
using namespace std;

class cc_to {
public:
	cc_to() {
		Engine engine = Engine(CC_TO);
	}
	cc_to(Engine* e) {
		engine = *e;
	}

	void init(Data* data);

	//write request
	/*
	call data_to::access with P_REQ request
	if return ok, the write is valid, then call data_to::write with W_REQ request
	*/
	RC update(string key, int value, thread::id tid, time_t ts);

	//read request
	/*
	call data_to::access with R_REQ request
	*/
	RC get(string key, int& value, thread::id tid, time_t ts);

	Engine engine;
private:
	//std::mutex latch;
};