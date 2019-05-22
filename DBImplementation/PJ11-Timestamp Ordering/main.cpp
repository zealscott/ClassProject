#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <thread>
#include <ctime>
#include <unistd.h>
#include "cc_to.h"
#include "data_to.h"
#include "global.h"
#include "structure.h"

using namespace std;

struct Lock
{
	string key;
	lock_t type;
	Lock(string k, lock_t t)
	{
		key = k;
		type = t;
	}
};

void initial_data(Engine &engine, vector<int> &datalist);
RC do_transaction1(cc_to &ccTO);
RC transaction1(cc_to &ccTO);
RC transaction2(cc_to &ccTO);
RC transaction3(cc_to &ccTO);
RC transaction4(cc_to &ccTO);
//RC release_lock(cc_lock& ccLock, vector<Lock> getLockList, thread::id tid);
time_t get_ts();

vector<int> datalist;
mutex latch;

int main()
{
	Engine engine = Engine(CC_TO);
	cc_to ccTO = cc_to(&engine);
	vector<std::thread> threads;

	//initial data_map
	initial_data(ccTO.engine, datalist);

	//start some threads and do transactions
	for (int i = 0; i < 5; i++)
	{
		int j = i % 3;
		if (j == 0)
		{
			threads.push_back(std::thread(do_transaction1, std::ref(ccTO)));
		}
		else if (j == 1)
		{
			threads.push_back(std::thread(transaction2, std::ref(ccTO)));
		}
		else if (j == 2)
		{
			threads.push_back(std::thread(transaction3, std::ref(ccTO)));
		}
		/*
		else if (j == 3) {
			threads.push_back(std::thread(transaction4, std::ref(ccLock)));
		}*/
	}
	for (auto &th : threads)
	{
		th.join();
	}

	for (int i = 0; i < datalist.size(); i++)
	{
		auto find = ccTO.engine.data_map.find(to_string(datalist[i]));
		if (find->second.data_.deleted)
		{
			cout << "Key: " << i << "   has been deleted " << endl;
		}
		cout << "Key: " << i << "   Value: " << find->second.data_.value << endl;
		//cout << find->second.owner.type;
	}
	system("pause");
	return 0;
}

void initial_data(Engine &engine, vector<int> &datalist)
{
	for (int i = 0; i < 6; i++)
	{
		data_to data = data_to();
		data.data_.value = i + 10;

		engine.data_map[to_string(i)] = data;
		datalist.push_back(i);
	}
}

RC do_transaction1(cc_to &ccTO)
{
	RC rc = RCOK;
	rc = transaction1(ccTO);
	if (rc == ABORT)
		cout << "thread: " << this_thread::get_id() << " is abort" << endl;
	while (rc != RCOK)
		rc = transaction1(ccTO);
	return rc;
}

RC transaction1(cc_to &ccTO)
{
	RC rc = RCOK;
	thread::id tid = this_thread::get_id();
	time_t ts = get_ts();
	int k1 = 0, k2 = 2;
	int v1, v2;
	rc = ccTO.get(to_string(k1), v1, tid, ts);
	if (rc != RCOK)
		return rc;
	rc = ccTO.get(to_string(k2), v2, tid, ts);
	if (rc != RCOK)
		return rc;
	if (v1 >= 2)
	{
		rc = ccTO.update(to_string(k1), v1 - 2, tid, ts);
		if (rc != RCOK)
			return rc;
		rc = ccTO.update(to_string(k2), v2 + 2, tid, ts);
		if (rc != RCOK)
			return rc;
	}
	return rc;
}

RC transaction2(cc_to &ccTO)
{
	RC rc = RCOK;
	thread::id tid = this_thread::get_id();
	time_t ts = get_ts();
	int k1 = 0, k2 = 4;
	int v1, v2;
	rc = ccTO.get(to_string(k1), v1, tid, ts);
	if (rc != RCOK)
		return rc;
	rc = ccTO.get(to_string(k2), v2, tid, ts);
	if (rc != RCOK)
		return rc;
	if (v1 >= 2)
	{
		rc = ccTO.update(to_string(k1), v1 - 2, tid, ts);
		if (rc != RCOK)
			return rc;
		rc = ccTO.update(to_string(k2), v2 + 2, tid, ts);
		if (rc != RCOK)
			return rc;
	}
	return rc;
}

RC transaction3(cc_to &ccTO)
{
	RC rc = RCOK;
	thread::id tid = this_thread::get_id();
	time_t ts = get_ts();
	int k1 = 1, k2 = 3, k3 = 5;
	int v1, v2, v3;
	rc = ccTO.get(to_string(k1), v1, tid, ts);
	if (rc != RCOK)
		return rc;
	rc = ccTO.get(to_string(k2), v2, tid, ts);
	if (rc != RCOK)
		return rc;
	rc = ccTO.get(to_string(k3), v3, tid, ts);
	if (rc != RCOK)
		return rc;
	rc = ccTO.update(to_string(k1), v1 * 2, tid, ts);
	if (rc != RCOK)
		return rc;
	rc = ccTO.update(to_string(k2), v2 + 5, tid, ts);
	if (rc != RCOK)
		return rc;
	rc = ccTO.update(to_string(k3), v3 + 10, tid, ts);
	if (rc != RCOK)
		return rc;
	return rc;
}

time_t get_ts()
{
	latch.lock();
	time_t ts = time(0);
	sleep(1);
	latch.unlock();
	return ts;
}

/*
RC release_lock(cc_lock& ccLock, vector<Lock> getLockList, thread::id tid) {
	RC rc = RCOK;
	for (int i = 0; i < getLockList.size(); i++) {
		rc=ccLock.lock_release(getLockList[i].type, tid, getLockList[i].key);
	}
	return rc;
}

RC transaction1(cc_lock& ccLock) {
	RC rc = RCOK;
	std::thread::id tid;
	tid = this_thread::get_id();
	vector<Lock> getLockList;
	int k1 = 0, k2 = 2;
	int v1, v2;
	rc = ccLock.get(to_string(k1), v1, tid);
	rc = ccLock.get(to_string(k2), v2, tid);
	if (v1 >= 2) {
		rc = ccLock.update(to_string(k1), v1 - 2, tid);
		Lock lock1 = Lock(to_string(k1), LOCK_EX);
		getLockList.push_back(lock1);
		rc = ccLock.update(to_string(k2), v2 + 2, tid);
		Lock lock2 = Lock(to_string(k2), LOCK_EX);
		getLockList.push_back(lock2);
	}
	rc = release_lock(ccLock, getLockList, tid);
	return rc;
}

RC transaction2(cc_lock& ccLock) {
	RC rc = RCOK;
	std::thread::id tid;
	tid = this_thread::get_id();
	vector<Lock> getLockList;
	int k1 = 2, k2 = 4;
	int v1, v2;
	rc = ccLock.get(to_string(k1), v1, tid);
	rc = ccLock.get(to_string(k2), v2, tid);
	if (v1 >= 2) {
		rc = ccLock.update(to_string(k1), v1 - 1, tid);
		Lock lock1 = Lock(to_string(k1), LOCK_EX);
		getLockList.push_back(lock1);
		rc = ccLock.update(to_string(k2), v2 + 1, tid);
		Lock lock2 = Lock(to_string(k2), LOCK_EX);
		getLockList.push_back(lock2);
	}
	rc = release_lock(ccLock, getLockList, tid);
	return rc;
}

RC transaction3(cc_lock& ccLock) {
	RC rc = RCOK;
	std::thread::id tid;
	tid = this_thread::get_id();
	vector<Lock> getLockList;
	int k1 = 1, k2 = 3, k3=5;
	int v1, v2, v3;
	rc = ccLock.get(to_string(k1), v1, tid);
	rc = ccLock.get(to_string(k2), v2, tid);
	rc = ccLock.get(to_string(k3), v3, tid);

	rc = ccLock.update(to_string(k1), v1 * 2, tid);
	Lock lock1 = Lock(to_string(k1), LOCK_EX);
	getLockList.push_back(lock1);

	rc = ccLock.update(to_string(k2), v2 + 5, tid);
	Lock lock2 = Lock(to_string(k2), LOCK_EX);
	getLockList.push_back(lock2);

	rc = ccLock.update(to_string(k3), v3 + 10, tid);
	Lock lock3 = Lock(to_string(k3), LOCK_EX);
	getLockList.push_back(lock3);
	
	rc = release_lock(ccLock, getLockList, tid);
	return rc;
}

RC transaction4(cc_lock& ccLock) {
	RC rc = RCOK;
	std::thread::id tid;
	tid = this_thread::get_id();
	vector<Lock> getLockList;

	ccLock.insert("6", 20, tid);
	datalist.push_back(6);

	
	ccLock.delete_("5", tid);
	Lock lock1 = Lock("5", LOCK_EX);
	getLockList.push_back(lock1);

	rc = release_lock(ccLock, getLockList, tid);
	return rc;
}*/