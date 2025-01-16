#include "ContactRecord.h"
#include <algorithm>

void ContactRecord::AddRecord(uint32_t number) {
	// 履歴に登録
	record_.push_back(number);
}

bool ContactRecord::CheckRecord(uint32_t number) {
	// 履歴に一致があればtrueを返す
	if (std::any_of(record_.begin(), record_.end(), [number](uint32_t record) {return record == number; })) {
		return true;
	}
	return false;
}

void ContactRecord::Clear() {
	// 履歴を抹消
	record_.clear();
}