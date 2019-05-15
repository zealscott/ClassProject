#pragma once
#include <thread>
#include "global.h"
#include "structure.h"
using namespace std;

class cc_lock
{
public:
	cc_lock() { Engine engine = Engine(CC_LOCK); }
	cc_lock(Engine *e) { engine = *e; }

	//获取锁
	RC lock_get(lock_t type, thread::id tid, Data& data);

	//释放锁
	RC lock_release(lock_t type, thread::id tid, Data& data);

	//更新操作
	RC update(string key, int value, thread::id tid);

	//删除操作
	RC delete_(string key, thread::id tid);

	//插入操作
	RC insert(string key, int value, thread::id tid);

	//读操作
	RC get(string key, int &value, thread::id tid);

	Engine engine;

private:
	//判断两个锁是否冲突
	bool conflict_lock(LockEntry lock1, LockEntry lock2);
	LockEntry *get_entry();
	void return_entry(LockEntry *entry);
};
