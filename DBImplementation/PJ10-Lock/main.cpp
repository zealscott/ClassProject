#include "cc_lock.h"
#include "global.h"
#include "structure.h"
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <vector>

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
RC transaction1(cc_lock &ccLock);
RC transaction2(cc_lock &ccLock);
RC transaction3(cc_lock &ccLock);
RC transaction4(cc_lock &ccLock);
RC release_lock(cc_lock &ccLock, vector<Lock> getLockList, thread::id tid);

vector<int> datalist;

int main()
{
	cc_lock ccLock = cc_lock();
	vector<std::thread> threads;

	//初始化数据
	initial_data(ccLock.engine, datalist);

	//开启多个线程，模拟并发事务
	for (int i = 0; i < 8; i++)
	{
		int j = i % 4;
		if (j == 0)
		{
			threads.push_back(std::thread(transaction1, std::ref(ccLock)));
		}
		else if (j == 1)
		{
			threads.push_back(std::thread(transaction2, std::ref(ccLock)));
		}
		else if (j == 2)
		{
			threads.push_back(std::thread(transaction3, std::ref(ccLock)));
		}
		else if (j == 3)
		{
			threads.push_back(std::thread(transaction4, std::ref(ccLock)));
		}
	}
	for (auto &th : threads)
	{
		th.join();
	}

	for (int i = 0; i < datalist.size(); i++)
	{
		auto find = ccLock.engine.data_map.find(to_string(datalist[i]));
		if (find->second.deleted)
		{
			cout << "Key: " << i << "   has been deleted " << endl;
		}
		cout << "Key: " << i << "   Value: " << find->second.value << endl;
		//cout << find->second.owner.type;
	}
	system("pause");
	return 0;
}

void initial_data(Engine &engine, vector<int> &datalist)
{
	for (int i = 0; i < 6; i++)
	{
		Data data;
		data.deleted = false;
		data.owner.type = LOCK_NONE;
		data.timestamp = 0;
		data.value = i + 10;

		engine.data_map[to_string(i)] = data;
		datalist.push_back(i);
	}
}

RC release_lock(cc_lock &ccLock, vector<Lock> getLockList, thread::id tid)
{
	RC rc = RCOK;
	for (int i = 0; i < getLockList.size(); i++)
	{
		auto find = ccLock.engine.data_map.find(getLockList[i].key);
		rc = ccLock.lock_release(getLockList[i].type, tid, find->second);
	}
	return rc;
}

RC transaction1(cc_lock &ccLock)
{
	RC rc = RCOK;
	std::thread::id tid;
	tid = this_thread::get_id();
	vector<Lock> getLockList;
	int k1 = 0, k2 = 2;
	int v1, v2;
	rc = ccLock.get(to_string(k1), v1, tid);
	rc = ccLock.get(to_string(k2), v2, tid);
	if (v1 >= 2)
	{
		rc = ccLock.update(to_string(k1), v1 - 2, tid);
		if (rc != NOT_FOUND)
		{
			Lock lock1 = Lock(to_string(k1), LOCK_EX);
			getLockList.push_back(lock1);
		}

		rc = ccLock.update(to_string(k2), v2 + 2, tid);
		if (rc != NOT_FOUND)
		{
			Lock lock2 = Lock(to_string(k2), LOCK_EX);
			getLockList.push_back(lock2);
		}
	}
	rc = release_lock(ccLock, getLockList, tid);
	return rc;
}

RC transaction2(cc_lock &ccLock)
{
	RC rc = RCOK;
	std::thread::id tid;
	tid = this_thread::get_id();
	vector<Lock> getLockList;
	int k1 = 2, k2 = 4;
	int v1, v2;
	rc = ccLock.get(to_string(k1), v1, tid);
	rc = ccLock.get(to_string(k2), v2, tid);
	if (v1 >= 2)
	{
		rc = ccLock.update(to_string(k1), v1 - 1, tid);
		if (rc != NOT_FOUND)
		{
			Lock lock1 = Lock(to_string(k1), LOCK_EX);
			getLockList.push_back(lock1);
		}

		rc = ccLock.update(to_string(k2), v2 + 1, tid);
		if (rc != NOT_FOUND)
		{
			Lock lock2 = Lock(to_string(k2), LOCK_EX);
			getLockList.push_back(lock2);
		}
	}
	rc = release_lock(ccLock, getLockList, tid);
	return rc;
}

RC transaction3(cc_lock &ccLock)
{
	RC rc = RCOK;
	std::thread::id tid;
	tid = this_thread::get_id();
	vector<Lock> getLockList;
	int k1 = 1, k2 = 3, k3 = 5;
	int v1, v2, v3;
	rc = ccLock.get(to_string(k1), v1, tid);
	rc = ccLock.get(to_string(k2), v2, tid);
	rc = ccLock.get(to_string(k3), v3, tid);

	rc = ccLock.update(to_string(k1), v1 * 2, tid);
	if (rc != NOT_FOUND)
	{
		Lock lock1 = Lock(to_string(k1), LOCK_EX);
		getLockList.push_back(lock1);
	}

	rc = ccLock.update(to_string(k2), v2 + 5, tid);
	if (rc != NOT_FOUND)
	{
		Lock lock2 = Lock(to_string(k2), LOCK_EX);
		getLockList.push_back(lock2);
	}

	rc = ccLock.update(to_string(k3), v3 + 10, tid);
	if (rc != NOT_FOUND)
	{
		Lock lock3 = Lock(to_string(k3), LOCK_EX);
		getLockList.push_back(lock3);
	}

	rc = release_lock(ccLock, getLockList, tid);
	return rc;
}

RC transaction4(cc_lock &ccLock)
{
	RC rc = RCOK;
	std::thread::id tid;
	tid = this_thread::get_id();
	vector<Lock> getLockList;

	rc = ccLock.insert("6", 20, tid);
	if (rc != ALREADY_EXIST)
	{
		datalist.push_back(6);
	}

	rc = ccLock.delete_("5", tid);
	if (rc != NOT_FOUND)
	{
		Lock lock1 = Lock("5", LOCK_EX);
		getLockList.push_back(lock1);
	}

	rc = release_lock(ccLock, getLockList, tid);
	return rc;
}