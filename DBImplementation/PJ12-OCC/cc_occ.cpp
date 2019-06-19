#include "cc_occ.h"

RC cc_occ::update(string key, int value, txn_man * txn)
{
	RC rc = RCOK;
	Data a = Data();
	Data* data = &a;
	data->value = value;
	auto find = engine.data_map.find(key);
	if (find != engine.data_map.end() && !find->second.deleted) {
		updateEntry * updateentry = new updateEntry();
		updateentry->data = &(find->second);
		updateentry->value = value;
		txn->wr_list.push_back(updateentry);
		txn->wr_cnt++;
	}
	return rc;
}

RC cc_occ::get(string key, int & value, txn_man * txn)
{
	RC rc = RCOK;

	auto find = engine.data_map.find(key);
	if (find != engine.data_map.end() && !find->second.deleted) {
		value = find->second.value;
		txn->rd_list.push_back(&(find->second));
		txn->rd_cnt++;
	}
	return rc;
}

RC cc_occ::commit(txn_man * txn)
{
	RC rc = RCOK;

	rc = occ_man.validate(txn);
	//cout << rc << endl;
	//cout << txn->tid << "  s:  " << txn->start_ts << "  e:  " << txn->end_ts << "  c:  " << txn->commit_ts <<endl;
	return rc;
}