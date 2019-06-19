#include "data_to.h"
#include<iostream>

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

void data_to::init(Data * data)
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
RC data_to::access(thread::id tid, TsType type, Data* data, time_t ts)
{
	RC rc = RCOK;
	latch.lock();
	if (type == R_REQ) {
		if (ts < data_.writestamp)
			rc = ABORT;
		else if (ts > min_pts) {
			buffer_req(R_REQ, tid, data, ts);
			rc = WAIT;
		}
		else {
			data->value = data_.value;
			if (data_.readstamp < ts)
				data_.readstamp = ts;
			rc = RCOK;
		}
	}
	else if (type == P_REQ) {
		if (ts < data_.readstamp||ts<data_.writestamp)
			rc = ABORT;
		else {
			buffer_req(P_REQ, tid, data, ts);
			rc = RCOK;
		}
	}
	else if (type == W_REQ) {
		rc = RCOK;
		if (ts > min_pts || ts > min_rts) {
			buffer_req(W_REQ, tid, data, ts);
			goto final;
		}
		else {
			data_.value = data->value;
			if (data_.writestamp < ts)
				data_.writestamp = ts;
			TOReqEntry* req = debuffer_req(P_REQ, tid);
			update_buffer();
		}
	}
	final:
	latch.unlock();
	return rc;
}

time_t data_to::cal_min(TsType type)
{
	TOReqEntry * queue;
	switch (type) {
		case R_REQ: queue = readreq; break;
		case P_REQ: queue = prereq; break;
		case W_REQ: queue = writereq; break;
		default: queue = readreq;
	}
	time_t new_min_pts = UINT64_MAX;
	TOReqEntry * req = queue;
	while (req != NULL) {
		if (req->ts < new_min_pts)
			new_min_pts = req->ts;
		req = req->next;
	}
	return new_min_pts;
}

//buffer the request which is valid but can not execute right now
void data_to::buffer_req(TsType type, thread::id tid, Data * data, time_t ts)
{
	TOReqEntry * req_entry= new TOReqEntry();
	req_entry->data = data;
	req_entry->tid = tid;
	req_entry->ts = ts;
	if (type == R_REQ) {
		req_entry->next = readreq;
		readreq = req_entry;
		if (req_entry->ts < min_rts)
			min_rts = req_entry->ts;
	}
	else if (type == W_REQ) {
		req_entry->next = writereq;
		writereq = req_entry;
		if (req_entry->ts < min_wts)
			min_wts = req_entry->ts;
	}
	else if (type == P_REQ) {
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
TOReqEntry * data_to::debuffer_req(TsType type, thread::id tid)
{
	TOReqEntry ** queue;
	TOReqEntry* return_queue=NULL;
	switch (type) {
		case R_REQ: queue = &readreq; break;
		case P_REQ: queue = &prereq; break;
		case W_REQ: queue = &writereq; break;
		default: queue = &readreq;
	}
	
	TOReqEntry * req = *queue;
	TOReqEntry * prev_req = NULL;
	if (req != NULL) {
		while (req->tid != tid) {
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
	if (type == P_REQ) {
		time_t new_min_pts = cal_min(P_REQ);
		if (new_min_pts > min_pts)
			min_pts = new_min_pts;
	}
	else if (type == R_REQ) {
		time_t new_min_rts = cal_min(R_REQ);
		if (new_min_rts > min_rts)
			min_rts = new_min_rts;
	}
	return return_queue;
}

/*
*delete TOReqEntry based on timestamp, if smaller than ts, delete and do the request, use for undate_buffer()
*/
TOReqEntry* data_to::delete_buffer(TsType type, time_t ts)
{
	TOReqEntry** queue;
	TOReqEntry* return_queue = NULL;
	switch (type) {
	case R_REQ: queue = &readreq; break;
	case P_REQ: queue = &prereq; break;
	case W_REQ: queue = &writereq; break;
	default: queue = &readreq;
	}
	TOReqEntry* req = *queue;
	TOReqEntry* prev_req = NULL;
	while (req != NULL) {
		if (req->ts <= ts) {
			if (prev_req != NULL)
				prev_req->next = req->next;
			else {
				*queue = (*queue)->next;
			}
			req->next = return_queue;
			return_queue = req;
			req = (prev_req == NULL) ? *queue : prev_req->next;
			if (type == P_REQ)
				preq_len--;
		}
		else {
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
	while (true) {
		time_t new_min_pts = cal_min(P_REQ);
		if (new_min_pts > min_pts)
			min_pts = new_min_pts;
		else
			break;
		TOReqEntry* ready_read = delete_buffer(R_REQ, min_pts);
		if (ready_read == NULL) 
			break;
		TOReqEntry* req = ready_read;
		while (req != NULL) {
			req->data = &data_;
			if (data_.readstamp < req->ts)
				data_.readstamp = req->ts;
			req = req->next;
		}

		time_t new_min_rts = cal_min(R_REQ);
		if (new_min_rts > min_rts)
			min_rts = new_min_rts;
		else
			break;
		TOReqEntry* ready_write = delete_buffer(W_REQ, min_rts);
		if (ready_write == NULL) 
			break;
		time_t young_ts = UINT64_MAX;
		TOReqEntry* young_req = NULL;
		req = ready_write;
		while (req != NULL) {
			TOReqEntry* tmp_req = debuffer_req(P_REQ, req->tid);
			if (req->ts > young_ts) {
				young_ts = req->ts;
				young_req = req;
			} //else loser = req;
			req = req->next;
		}
		// perform write.
		data_.value = young_req->data->value;
		if (data_.writestamp < young_req->ts)
			data_.writestamp = young_req->ts;
	}
}

void TOReqEntry::init()
{
	data = &(Data());
	ts = 0;
	next = NULL;
}

TOReqEntry::TOReqEntry()
{
	tid = this_thread::get_id();
	data=&(Data());
	ts = 0;
	next = NULL;
}
