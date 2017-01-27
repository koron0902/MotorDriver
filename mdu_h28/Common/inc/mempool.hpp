#pragma once
#ifndef COMMON_MEMPOOL_H_
#define COMMON_MEMPOOL_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>

namespace common {


//[begin,end)の中に入っているかの確認
static inline bool InRange(void *cmp,void* begin,void* end){
	bool low= (begin<=cmp);
	bool high=(cmp<end);
	return low&&high;
}

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

/* 固定用メモリーダンプ
 */
template <size_t size>class LockedPool{
	size_t pos;
	char mem[size];
public:
	LockedPool(){
		pos=0;
	}

	void* CreatePointer(size_t sz){
		const size_t next =pos+sz;
		const size_t now=pos;
		if (next<size){
			pos=next;
			return &mem[now];
		}else{
			return nullptr;
		}
	}

	void ReleasePointer(void*& ptr){
		//解放処理としては何もしない。
		ptr=nullptr;
	}

	const char* Clone(const char* str){
		char* ptr= (char*)CreatePointer(strlen(str)+1);
		strcpy(ptr,str);
		return ptr;
	}

	size_t CountUsedByte()const{
		return pos;
	}

	size_t CountFreeByte()const{
		return size-pos;
	}

	size_t CountAllByte()const{
		return size;
	}

};

//テキスト用動的メモリ
template <size_t size>class TextPool{
	char buffer[size];



};

} /* namespace common */

#endif /* COMMON_MEMPOOL_H_ */
