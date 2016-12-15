#include <chip.hpp>
#include <SysTick.hpp>

using namespace common;

namespace Device {
namespace Tick {

static uint64_t tick;
static constexpr uint64_t bit24=(1u<<10);
static constexpr uint64_t mask=bit24-1;

//システムモニタ用タイマーの初期化
void Init() {
	auto *p=SysTick;
	p->LOAD=mask;
	p->VAL=mask;
	tick = 0;
}

uint64_t Tick() {
	auto *p=SysTick;
	return tick+(bit24-(p->LOAD));
}

uint64_t TickUs(){
	return Tick()*1_MHz/SystemCoreClock;
}

uint64_t TickMs(){
	return Tick()*1_KHz/SystemCoreClock;
}

void DelayUs(uint64_t us){
	uint64_t time= TickUs()+us;
	while (TickUs()<time);
}

void DelayMs(uint64_t ms){
	uint64_t time=TickMs()+ms;
	while (TickMs()<time);
}

extern "C" void SysTick_Handler() {
	auto *p=SysTick;
	p->LOAD=mask;
	tick+=bit24;
}

}
}
