#ifndef MIDDLE_COUNTER_H_
#define MIDDLE_COUNTER_H_

#include <fix.hpp>

namespace common {
using namespace common;

struct Counter {
	uint32_t begin;
	uint32_t end;
	uint32_t cycle;
	uint32_t busy;
public:
	Counter();
	void Begin();//計測開始
	void End();//計測終了
	fix32 Account()const;

	inline uint32_t GetCycle()const{
		return cycle;
	}

	inline uint32_t GetBusy()const{
		return busy;
	}

};

} /* namespace mid */

#endif /* MIDDLE_COUNTER_H_ */
