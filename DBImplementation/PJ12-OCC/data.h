#pragma once
#include"global.h"
#include<ctime>
#include<mutex>

struct Data {
	int value;
	bool deleted;
	std::mutex latch;

	time_t commitstamp;
	time_t readstamp;
	time_t writestamp;
	bool commit;

	//time_t commitstamp;

	Data() {
		value = -1;
		deleted = false;
		readstamp = -1;
		writestamp = -1;
		commitstamp = -1;
	}

	void set(Data* data1, Data* data2) {
		data1->deleted = data2->deleted;
		data1->readstamp = data2->readstamp;
		data1->writestamp = data2->writestamp;
		data1->commitstamp = data2->commitstamp;
		data1->value = data2->value;
	}
	Data& operator=(const Data& d) {
		set(this, (Data*)& d);
		return *this;
	}
	Data(const Data& d) {
		*this = d;
	}
};