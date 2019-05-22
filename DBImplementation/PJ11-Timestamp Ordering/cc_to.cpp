#include "cc_to.h"

void cc_to::init(Data *data)
{
}

RC cc_to::update(string key, int value, thread::id tid, time_t ts)
{
	RC rc = RCOK;
	Data t;
	Data *data = &t;
	//data->init();
	data->value = value;
	auto find = engine.data_map.find(key);
	if (find != engine.data_map.end() && !find->second.data_.deleted)
	{
		rc = find->second.access(tid, P_REQ, data, ts);
		if (rc == RCOK)
		{
			rc = find->second.access(tid, W_REQ, data, ts);
		}
		else
			return rc;
	}
	return rc;
}

RC cc_to::get(string key, int &value, thread::id tid, time_t ts)
{
	RC rc = RCOK;
	Data t;
	Data *data = &t;

	auto find = engine.data_map.find(key);
	if (find != engine.data_map.end() && !find->second.data_.deleted)
	{
		rc = find->second.access(tid, R_REQ, data, ts);
		if (rc == RCOK)
			value = data->value;
		else
			return rc;
	}
	return rc;
}