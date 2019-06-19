#pragma once
#include"global.h"
#include"data.h"
#include<thread>
#include<ctime>
#include<vector>

class updateEntry {
public:
	Data * data;
	int value;
};

class txn_man {
public:
	txn_man() {
		start_ts = INT64_MAX;
		end_ts = INT64_MAX;
		commit_ts = INT64_MAX;
		tid = this_thread::get_id();

		rd_cnt = 0;
		wr_cnt = 0;
	}

	//begin time of the read phase
	time_t start_ts;

	//end time of the read phase
	time_t end_ts;

	time_t commit_ts;

	thread::id tid;

	//buffer the update data of the write operation
	vector<updateEntry*> wr_list;

	//record the read data
	vector<Data*> rd_list;

	int rd_cnt;
	int wr_cnt;
};