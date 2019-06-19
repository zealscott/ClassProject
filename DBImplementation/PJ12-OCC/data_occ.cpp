#include "data_occ.h"

data_occ::data_occ()
{
	his_len = 0;
	active_len = 0;
	history = NULL;
	active = NULL;
}

void data_occ::init()
{
	his_len = 0;
	active_len = 0;
	history = NULL;
	active = NULL;
}

void data_occ::update_data(txn_man *txn, setEntry *wset,bool valid)
{
	latch.lock();
	setEntry *activeEntry = active;
	setEntry *prev = NULL;
	// search active list to get txn
	while (activeEntry->txn != txn)
	{
		prev = activeEntry;
		activeEntry = activeEntry->next;
	}
	if (prev != NULL)
		prev->next = activeEntry->next;
	else
		active = activeEntry->next;
	active_len--;
	if (valid)
	{
		// get commit time
		time_t ts = time(0);
		txn->commit_ts = ts;
		// add to history
		wset->next = history;
		history = wset;
		his_len++;
		// update data
		for (int i = 0; i < wset->set_size; i++)
			wset->datalist[i]->value = wset->values[i];
	}

	latch.unlock();
}

/*
 1.get end_ts, active txn list, history txn list, and add itself to active list
 2.validate whether the rset of this txn overlap with the wset in history list
 3.validate whether the rset and wset of this txn overlap with the wset in active list
 4.if have conflict, then abort and remove it from active
 5.if have no conflict, then apply write to the data
 6.remove itself from active and add it to history
*/
RC data_occ::validate(txn_man *txn)
{
	RC rc = RCOK;
	txn->end_ts = time(0);
	time_t start_ts = txn->start_ts;
	time_t end_ts = txn->end_ts;
	bool valid = true;

	setEntry *wset = new setEntry();
	setEntry *rset = new setEntry();
	get_rw_set(txn, rset, wset);

	bool read_only = (wset->set_size == 0);
	setEntry *historyEntry = history;
	setEntry *activeEntry = active;

	// modify history and active need to lock
	latch.lock();
	if (!read_only)
	{
		active_len++;
		wset->next = active;
		active = wset;
	}
	latch.unlock();

	while (historyEntry != NULL)
	{
		bool overlap = is_overlap(historyEntry, start_ts, end_ts);
		if (overlap)
		{
			if (!test_validate(historyEntry, rset))
			{
				if (!read_only)
					update_data(txn, wset, false);
				rc = ABORT;
				return rc;
			}
		}
		historyEntry = historyEntry->next;
	}

	while (activeEntry != NULL)
	{
		if (!test_validate(activeEntry, rset) || test_validate(activeEntry, wset))
		{
			if (!read_only)
				update_data(txn, wset, false);
			rc = ABORT;
			return rc;
		}
		activeEntry = activeEntry->next;
	}
	update_data(txn, wset, true);
	return rc;
}

/*
test if two set are confilct
*/
bool data_occ::test_validate(setEntry *set1, setEntry *set2)
{
	if (set1->txn->tid == set2->txn->tid)
		return true;
	for (int i = 0; i < set1->set_size; i++)
		for (int j = 0; j < set2->set_size; j++)
		{
			if (set1->datalist[i] == set2->datalist[j])
			{
				return false;
			}
		}
	return true;
}

/*
get the rset and wset of a txn
*/
RC data_occ::get_rw_set(txn_man *txn, setEntry *&rset, setEntry *&wset)
{
	//wset = (setEntry*)mem_allocator.alloc(sizeof(setEntry), 0);
	wset->set_size = txn->wr_cnt;
	rset->set_size = txn->rd_cnt;
	wset->txn = txn;
	rset->txn = txn;
	for (int i = 0; i < txn->wr_cnt; i++)
	{
		wset->datalist.push_back(txn->wr_list[i]->data);
		wset->values.push_back(txn->wr_list[i]->value);
	}
	for (int j = 0; j < txn->rd_cnt; j++)
	{
		rset->datalist.push_back(txn->rd_list[j]);
	}
	return RCOK;
}

/*
test if a txn is overlap with this time scope
*/
bool data_occ::is_overlap(setEntry *set, time_t start, time_t end)
{

	if (set->txn->commit_ts <= start)
		return false;
	else if (set->txn->commit_ts <= end)
		return true;
	else
		return false;
}
