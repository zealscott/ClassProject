#pragma once
#include <thread>
#include <time.h>
#include <mutex>
#include <vector>
#include "global.h"
#include "data.h"
#include "txn.h"
class setEntry
{
public:
	setEntry()
	{
		txn = NULL;
		set_size = 0;
		//datalist = NULL;
		next = NULL;
	}
	txn_man *txn;
	int set_size;
	//Data ** datalist;
	vector<int> values;
	vector<Data *> datalist;
	setEntry *next;
};

class data_occ
{
public:
	data_occ();
	void init();
	RC validate(txn_man *txn);
	RC get(txn_man *txn, setEntry *set);

	void set(data_occ *data1, data_occ *data2)
	{
		data1->his_len = data2->his_len;
		data1->history = data2->history;
		data1->active_len = data2->active_len;
		data1->active = data2->active;
	}
	data_occ &operator=(const data_occ &d)
	{
		set(this, (data_occ *)&d);
		return *this;
	}
	data_occ(const data_occ &d)
	{
		*this = d;
	}
	int his_len;
	int active_len;

private:
	bool test_validate(setEntry *set1, setEntry *set2);
	RC get_rw_set(txn_man *txn, setEntry *&rset, setEntry *&wset);
	bool is_overlap(setEntry *set, time_t start, time_t end);
	void update_data(txn_man *txn, setEntry *wset, bool valid);

	//wset of committed txn, new to old
	setEntry *history;
	//wset of active txn
	setEntry *active;

	mutex latch;
};