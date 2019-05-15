#include <iostream>
#include <string>
#include <thread>

#include "cc_lock.h"
#include "global.h"
#include "structure.h"

RC cc_lock::lock_get(lock_t type, thread::id tid, Data &data)
{
	RC rc = RCOK;
	LockEntry lock1 = data.owner;
	LockEntry lock2;
	lock2.tid = tid;
	lock2.type = type;
	if (!conflict_lock(lock1, lock2))
	{
		if (lock1.tid == lock2.tid)
			data.owner.type = min(lock1.type, lock2.type);
		else
		{
			data.latch.lock();
			data.owner = lock2;
		}
	}
	else
	{
		data.waitlist.push(&lock2);
		data.latch.lock();
		data.owner = lock2;
		data.waitlist.pop();
	}
	return rc;
}

/**
 * 判断数据项上有没有阻塞的锁请求
 */
RC cc_lock::lock_release(lock_t type, thread::id tid, Data &data)
{
	RC rc = RCOK;
	if (data.waitlist.empty())
	{
		data.latch.unlock();
		data.owner.type = LOCK_NONE;
		data.owner.tid = tid;
	}
	else
	{
		LockEntry *nowEntry = data.waitlist.front();
		data.owner.tid = nowEntry->tid;
		data.owner.type = nowEntry->type;
		data.latch.unlock();
	}
	return rc;
}

/** 如果两个线程都是读锁，或者没有锁，则不冲突；
 *  否则都会发生冲突
 *  lock1是数据上的锁，lock2是需要加的锁
 */
bool cc_lock::conflict_lock(LockEntry lock1, LockEntry lock2)
{
	if (lock1.tid == lock2.tid)
		return false;
	if (lock1.type == LOCK_EX || lock2.type == LOCK_EX)
		return true;
	return false;
}

RC cc_lock::update(string key, int value, thread::id tid)
{
	RC rc = RCOK;
	auto find = engine.data_map.find(key);
	if (find != engine.data_map.end() && !find->second.deleted)
	{
		rc = lock_get(LOCK_EX, tid, find->second);
		if (rc == RCOK)
		{
			find->second.value = value;
		}
		else
			return rc;

		// rc = lock_release(LOCK_EX, tid, find->second);
	}
	else
	{
		rc = NOT_FOUND;
	}
	return rc;
}

RC cc_lock::delete_(string key, thread::id tid)
{
	RC rc = RCOK;
	auto find = engine.data_map.find(key);
	if (find != engine.data_map.end() && !find->second.deleted)
	{
		rc = lock_get(LOCK_EX, tid, find->second);
		if (rc == RCOK)
		{
			find->second.deleted = true;
		}
		else
			return rc;

		// rc = lock_release(LOCK_EX, tid, find->second);
	}
	else
	{
		rc = NOT_FOUND;
	}
	return rc;
}

RC cc_lock::insert(string key, int value, thread::id tid)
{
	RC rc = RCOK;
	auto find = engine.data_map.find(key);
	if (find != engine.data_map.end() && !find->second.deleted)
	{
		rc = ALREADY_EXIST;
	}
	else
	{
		Data data;
		data.deleted = false;
		data.value = value;
		data.owner.type = LOCK_NONE;
		// data->value = value;
		engine.data_map[key] = data;
	}
	return rc;
}

RC cc_lock::get(string key, int &value, thread::id tid)
{
	RC rc = RCOK;
	auto find = engine.data_map.find(key);

	if (find != engine.data_map.end() && !find->second.deleted)
	{
		rc = lock_get(LOCK_SH, tid, find->second);
		value = find->second.value;
	}
	else
	{
		rc = NOT_FOUND;
	}
	return rc;
}
