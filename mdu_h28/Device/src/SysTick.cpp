#include <chip.hpp>
#include <SysTick.hpp>
#include <configuration.hpp>
using namespace common;

namespace Device {
namespace Tick {

static uint64_t tick;
static constexpr uint64_t bit24 = (1u << 24);
static constexpr uint64_t mask = bit24 - 1;

static inline void interrupt_disable(){
	//NVIC_DisableIRQ(SysTick_IRQn);エラー発生
}

static inline void interrupt_enable(){
	interrupt_disable();
	NVIC_SetPriority(SysTick_IRQn,PrioritySysTick);
	//NVIC_EnableIRQ(SysTick_IRQn);
}


//システムモニタ用タイマーの初期化
void Init() {
	interrupt_enable();
	SysTick_Config(mask);
	tick = 0;
}

uint64_t Tick() {
	auto *p = SysTick;
	return tick + (bit24 - (p->LOAD));
}

uint64_t TickUs() {
	return Tick() *100000/SystemCoreClock;
}

uint64_t TickMs() {
	return Tick() *1000/  SystemCoreClock;
}

uint64_t TickS(){
	return Tick()/SystemCoreClock;
}

void DelayUs(uint64_t us) {
	uint64_t time = TickUs() + us;
	while (TickUs() < time) {
		__NOP();
	}
}

void DelayMs(uint64_t ms) {
	uint64_t time = TickMs() + ms;
	while (TickMs() < time) {
		__NOP();
	}
}

extern "C" void SysTick_Handler() {
	//SysTick_Config(mask);
	tick += bit24;
}

}
}
