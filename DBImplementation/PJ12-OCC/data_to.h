#pragma once
#include<thread>
#include<time.h>
#include<mutex>
#include "global.h"
#include "data.h"
//#include"structure.h"

using namespace std;



struct TOReqEntry {
	std::thread::id tid;
	Data * data;
	time_t ts;
	TOReqEntry * next;

	void init();
	TOReqEntry();
};

class data_to {
public:
	data_to();
	data_to(Data * data);
	void init(Data * data);
	
	//process the request
	RC access(thread::id tid, TsType type, Data* data, time_t ts);

	Data data_;

	void set(data_to* data1, data_to* data2) {
		data1->preq_len = data2->preq_len;
		data1->min_pts = data2->min_pts;
		data1->min_rts = data2->min_rts;
		data1->min_wts = data2->min_wts;
		data1->prereq = data2->prereq;
		data1->readreq = data2->readreq;
		data1->writereq = data2->writereq;
		data1->data_ = data2->data_;
	}
	data_to& operator=(const data_to& d) {
		set(this, (data_to*)& d);
		return *this;
	}
	data_to(const data_to& d) {
		*this = d;
	}

private:
	time_t cal_min(TsType type);
	void buffer_req(TsType type, thread::id tid, Data * data, time_t ts);
	TOReqEntry * debuffer_req(TsType type, thread::id tid);
	TOReqEntry* delete_buffer(TsType type, time_t ts);
	void update_buffer();

	TOReqEntry* readreq;
	TOReqEntry* writereq;
	TOReqEntry* prereq;

	time_t min_rts;
	time_t min_wts;
	time_t min_pts;

	std::mutex latch;
	int preq_len;
};