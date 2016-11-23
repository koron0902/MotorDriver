/*
 * Mempool.h
 *
 *  Created on: 2016/11/06
 *      Author: TERU
 */

#ifndef COMMON_MEMPOOL_H_
#define COMMON_MEMPOOL_H_

#include <stdint.h>
#include <stddef.h>

namespace common {

/* size 確保する大きさ
 * num　確保する個数
 */

template<size_t size, unsigned num> class MemPool {
	union item {
		uint8_t dummy[size];
		item* next;
	};
	item data[num];
	item *top; //使っていない要素の単方向リスト
private:
	void Push(item* it) {
		it->next = top;
		top = it;
	}
	item* Pop() {
		item* temp = top;
		top = top->next;
		temp->next = nullptr;/*バグ防止*/
		return temp;
	}

public:
	bool IsFull() const {
		return top == nullptr;
	}

	MemPool() {
		//全ての単方向リストを連結する
		for (unsigned int i = 0; i < num; i++) {
			Push(&data[i]);
		}
	}

	MemPool(const MemPool&)=delete;

	void* CreatePointer() {
		return Pop();
	}

	void ReleasePointer(void* ptr) {
		if (ptr != nullptr) {
			Push((item*) ptr);
		}
	}

	int CountArea() const {
		return num;
	}

	int CountAreaByte() const {
		return num * size;
	}

	unsigned int CountFree() const {
		unsigned int n = 0;
		for (item* it = top; it != nullptr; it = it->next) {
			n++;
		}
		return n;
	}

	size_t CountFreeByte() const {
		return size * CountFree();
	}

	unsigned int CountUsed() const {
		return num - CountFree();
	}

	size_t CountUsedByte() const {
		return size * CountUsed();
	}



};

} /* namespace common */

#endif /* COMMON_MEMPOOL_H_ */
