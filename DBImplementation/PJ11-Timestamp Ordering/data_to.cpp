#include "data_to.h"
#include <iostream>

using namespace std;

data_to::data_to()
{
	data_ = Data();
	min_wts = INT64_MAX;
	min_rts = INT64_MAX;
	min_pts = INT64_MAX;
	readreq = NULL;
	writereq = NULL;
	prereq = NULL;
	preq_len = 0;
}

void data_to::init(Data *data)
{
	data_ = Data();
	min_wts = INT64_MAX;
	min_rts = INT64_MAX;
	min_pts = INT64_MAX;
	readreq = NULL;
	writereq = NULL;
	prereq = NULL;
	preq_len = 0;
}

//process the request
/*
for read:
	if transaction ts < data.writestamp, abort
	else if ts>min_pts, add it to readreq
	else execute it
for pre_write:
	if transaction ts < data.readstamp or writestamp, abort
	else add it to prereq
for write:
	if ts>min_pts or min_rts, add it to writereq
	else execute and call update_buffer()
*/
RC data_to::access(thread::id tid, TsType type, Data *data, time_t ts)
{
	RC rc = RCOK;
	if (type == R_REQ)
	{
		if (data->writestamp > ts)
			rc = RCERROR;
		else if (ts > min_pts)
		{
			buffer_req(type, tid, data, ts);
			// rc = WAIT;
		}
		else // execute
		{
			data->readstamp = ts;
		}
	}
	else if (type == P_REQ)
	{
		if (ts < data->writestamp)
			rc = RCERROR;
		else
		{
			buffer_req(type, tid, data, ts);
			// rc = WAIT;
		}
	}
	else
	{
		if (ts > min_pts || ts > min_rts)
		{
			buffer_req(type, tid, data, ts);
			// rc = WAIT;
		}
		else
		{
			// pop from pre write entry
			debuffer_req(P_REQ, tid);
			data->writestamp = ts;
			update_buffer();
		}
	}
	return rc;
}

time_t data_to::cal_min(TsType type)
{
	TOReqEntry *queue;
	switch (type)
	{
	case R_REQ:
		queue = readreq;
		break;
	case P_REQ:
		queue = prereq;
		break;
	case W_REQ:
		queue = writereq;
		break;
	default:
		queue = readreq;
	}
	time_t new_min_pts = UINT64_MAX;
	TOReqEntry *req = queue;
	while (req != NULL)
	{
		if (req->ts < new_min_pts)
			new_min_pts = req->ts;
		req = req->next;
	}
	return new_min_pts;
}

// buffer the request which is valid but can not execute right now
// update min ts
void data_to::buffer_req(TsType type, thread::id tid, Data *data, time_t ts)
{
	TOReqEntry *req_entry = new TOReqEntry();
	req_entry->data = data;
	req_entry->tid = tid;
	req_entry->ts = ts;
	if (type == R_REQ)
	{
		req_entry->next = readreq;
		readreq = req_entry;
		if (req_entry->ts < min_rts)
			min_rts = req_entry->ts;
	}
	else if (type == W_REQ)
	{
		req_entry->next = writereq;
		writereq = req_entry;
		if (req_entry->ts < min_wts)
			min_wts = req_entry->ts;
	}
	else if (type == P_REQ)
	{
		preq_len++;
		req_entry->next = prereq;
		prereq = req_entry;
		if (req_entry->ts < min_pts)
			min_pts = req_entry->ts;
	}
}

/*
delete TOReqEntry based on tid, when a write request is executed, the related pre-write should be deleted
*/
TOReqEntry *data_to::debuffer_req(TsType type, thread::id tid)
{
	TOReqEntry **queue;
	TOReqEntry *return_queue = NULL;
	switch (type)
	{
	case R_REQ:
		queue = &readreq;
		break;
	case P_REQ:
		queue = &prereq;
		break;
	case W_REQ:
		queue = &writereq;
		break;
	default:
		queue = &readreq;
	}

	TOReqEntry *req = *queue;
	TOReqEntry *prev_req = NULL;
	if (req != NULL)
	{
		while (req->tid != tid)
		{
			prev_req = req;
			req = req->next;
		}
		if (prev_req != NULL)
			prev_req->next = req->next;
		else
			*queue = req->next;
		if (type == P_REQ)
			preq_len--;
		req->next = return_queue;
		return_queue = req;
	}
	if (type == P_REQ)
	{
		time_t new_min_pts = cal_min(P_REQ);
		if (new_min_pts > min_pts)
			min_pts = new_min_pts;
	}
	else if (type == R_REQ)
	{
		time_t new_min_rts = cal_min(R_REQ);
		if (new_min_rts > min_rts)
			min_rts = new_min_rts;
	}
	return return_queue;
}

/*
* delete TOReqEntry based on timestamp, if smaller than ts, delete and do the request, use for undate_buffer()
*/
TOReqEntry *data_to::delete_buffer(TsType type, time_t ts)
{
	TOReqEntry **queue;
	TOReqEntry *return_queue = NULL;
	switch (type)
	{
	case R_REQ:
		queue = &readreq;
		break;
	case P_REQ:
		queue = &prereq;
		break;
	case W_REQ:
		queue = &writereq;
		break;
	default:
		queue = &readreq;
	}
	// two pointer to identify queue
	TOReqEntry *req = *queue;
	TOReqEntry *prev_req = NULL;
	while (req != NULL)
	{
		// find entry that smaller than ts timestamp
		if (req->ts <= ts)
		{
			if (prev_req != NULL)
				prev_req->next = req->next;
			else
			{
				*queue = (*queue)->next;
			}
			// current req is add to the front of return_queue
			req->next = return_queue;
			return_queue = req;
			req = (prev_req == NULL) ? *queue : prev_req->next;
			if (type == P_REQ)
				preq_len--;
		}
		else
		{
			prev_req = req;
			req = req->next;
		}
	}
	return return_queue;
}

//when a write req is executed, then update prereq and readreq, see which request can be executed
/*
while(true){
1. calculate the new min_pts,
2.a if it not changed, then break;
2.b else if changed, find the read requests in readreq which has a smaller ts than new min_pts(use delete_buffer), and execute them;
3. then calculate the new min_rts;
4.a if it not changed, then break;
4.b else find the write requests in writereq which has a smaller ts than new min_rts(use delete_buffer), and execute the write request with the largest timestamp;
5. delete the related pre_write request use debuffer_req
}
*/
void data_to::update_buffer()
{
	while (true)
	{
		time_t new_min_pts = cal_min(P_REQ);
		if (new_min_pts == min_pts)
			break;
		else
		{
			min_pts = new_min_pts;
			TOReqEntry *readExcuteEntry = delete_buffer(R_REQ, min_pts);
			while (readExcuteEntry != NULL)
			{
				if (readExcuteEntry->data->readstamp < readExcuteEntry->ts)
					readExcuteEntry->data->readstamp = readExcuteEntry->ts;
				readExcuteEntry = readExcuteEntry->next;
			}
			time_t new_min_rts = cal_min(R_REQ);
			if (new_min_rts == min_rts)
				break;
			else
			{
				min_rts = new_min_rts;
				TOReqEntry *writeExcuteEntry = delete_buffer(W_REQ, min_rts);
				while (writeExcuteEntry != NULL)
				{
					if (writeExcuteEntry->data->readstamp < writeExcuteEntry->ts)
						writeExcuteEntry->data->readstamp = writeExcuteEntry->ts;
					writeExcuteEntry = writeExcuteEntry->next;
				}
			}
		}
	}
}

void TOReqEntry::init()
{
	Data t;
	Data *data = &t;
	ts = 0;
	next = NULL;
}

TOReqEntry::TOReqEntry()
{
	tid = this_thread::get_id();
	Data t;
	Data *data = &t;
	ts = 0;
	next = NULL;
}
