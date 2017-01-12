/*
 * MessageQueue.cpp
 *
 *  Created on: 2016/11/23
 *      Author: hamus
 */

#include <message_queue.h>

using namespace std;

namespace common {

MessageQueue::MessageQueue() :
		que(Capacity) {
}

MessageQueue::~MessageQueue() {
	//特に何もしない
}

void MessageQueue::Insert(const process_t& pro, void* ptr ,
		int behavior) {
	//make pair and throw
	pair_t pair{pro,ptr};
	Insert(pair, behavior);
}

void MessageQueue::Insert(const pair_t& pair, int behavior) {
	switch (behavior) {
	case Continue:
		que.push_back(pair);
		break;
	case Quick:
		que.push_front(pair);
		break;
	default:
		//何もしない
		break;
	}
}

bool MessageQueue::Process(){
	if (!que.empty()){
		auto item= que.front();
		//先頭の処理を解決する
		process_t func= get<0>(item);
		void* ptr= get<1>(item);
		int res=func(ptr);
		//先頭を破棄
		que.pop_front();
		//再度利用するかどうか
		Insert(item,res);
		return !que.empty();
	}else{
		return false;
	}
}

} /* namespace App */
