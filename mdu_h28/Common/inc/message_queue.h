#pragma once
#ifndef COMMON_MESSAGEQUEUE_H_
#define COMMON_MESSAGEQUEUE_H_

#include <functional>
#include <deque>
#include <stdint.h>
#include <stddef.h>
#include <tuple>
namespace common {

class MessageQueue {
public:
	constexpr static unsigned int Capacity = 32u; //デフォルトのキューの最高深度
	//振る舞いを決める
	constexpr static int Delete = 0; //次回から呼び出されない
	constexpr static int Continue = 1; //末尾に挿入
	constexpr static int Quick = 2; //先頭に挿入
	using process_t=std::function<int(void*)>;
	using pair_t=std::tuple<process_t,void*>;
private:
	std::deque<pair_t> que;
public:
	MessageQueue();
	virtual ~MessageQueue();
	void Insert(const process_t& pro,void* ptr=nullptr,int behavior=Continue);
	void Insert(const pair_t&,int behavior=Continue);

	bool Process();
	void ProcessAll(){
		while (Process());
	}

	bool IsEmpty()const{
		return que.empty();
	}

	int Depth()const{
		return que.size();
	}

	//process内から呼び出さないこと
	void Claer(){
		que.clear();
	}



};

} /* namespace App */

#endif /* COMMON_MESSAGEQUEUE_H_ */
