#include <counter.hpp>
#include <SysTick.hpp>
#include <type.hpp>

/* 本来はmidに入れるべきだが普遍的なものなのでcommonに入れている。
 */

namespace common {
using namespace Device::Tick;
using namespace common;

Counter::Counter() :
		begin(0), end(0), cycle(0), busy(0) {
}

void Counter::Begin() {
	auto diff=[](uint32_t begin,uint32_t end)->uint32_t{
		if (end>=begin){
			return end-begin;
		}else{
			return UINT32_MAX-begin+end;
		}
	};

	uint32_t now= Tick()&UINT32_MAX;
	//前回の使用率を計算する。
	cycle=diff(begin,now);
	busy=diff(begin,end);

	//更新する
	begin=now;
}
void Counter::End() {
	//更新する
	end=Tick()&UINT32_MAX;
}

fix32 Counter::Account()const{
	q32_t temp=((uint64_t)cycle<<32)/busy;
	return fix32::CreateQ32(temp);
}




} /* namespace mid */
