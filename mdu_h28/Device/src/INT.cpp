#include "INT.hpp"

#include <chip.hpp>

namespace Device {
namespace INT {
using namespace Port;

const uint32_t IntMax = 8;
callback_t handlers[IntMax];

static inline void EnableInt(INT_ID id, uint8_t priority = 254);
static inline void DiseableInt(INT_ID id);
static inline void ClrIntSta(INT_ID id);

void Init() {
	//clock supply and clear reg.
	Chip_PININT_Init(LPC_GPIO_PIN_INT);
	for (uint32_t i = 0; i < IntMax; i++) {
		handlers[i] = nullptr;
	}
	//あとでmidに入れる関数

	//GPIO Initialize
	SetDin(HoleU);
	SetDin(HoleV);
	SetDin(HoleW);
	//登録する
	SetInt(HoleU, INT_ID::INT0);
	SetInt(HoleV, INT_ID::INT1);
	SetInt(HoleW, INT_ID::INT2);
	//割り込み先を登録
	auto func=[](){
		return;
	};
	SetHandler(INT_ID::INT0,func);
	SetHandler(INT_ID::INT1,func);
	SetHandler(INT_ID::INT2,func);
}

void SetHandler(INT_ID id, const callback_t& func , uint8_t priority) {
	handlers[(uint32_t) id] = func;
	if (func != nullptr) {
		EnableInt(id, priority);
	} else {
		DiseableInt(id);
	}
}

void SetInt(PortData data, INT_ID idx) {
	Chip_INMUX_PinIntSel(data.pin, data.port, (uint32_t) idx);
}

void ModeEdge(INT_ID id) {
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, (uint32_t) id);
}

void ModeHigh(INT_ID id) {
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, (uint32_t) id);
}

void ModeLow(INT_ID id) {
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, (uint32_t) id);
}

void ModeChanged(INT_ID id) {
	ModeEdge(id);
	ModeHigh(id);
	ModeLow(id);
}

static inline IRQn GetIRQ(INT_ID id) {
	//失敗すると深刻なエラーになるので値域を確認する。
	long t = (long) id;
	if (t < 8) {
		return (IRQn) ((long) PIN_INT0_IRQn + t);
	} else {
		return WDT_IRQn; //エラー
	}
}

//割り込み状態を削除する
static inline void ClrIntSta(INT_ID id) {
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, (uint32_t) id);
}

//割り込み許可
static inline void  EnableInt(INT_ID id, uint8_t priority) {
	auto irq = GetIRQ(id);
	if (irq != WDT_IRQn) {
		NVIC_ClearPendingIRQ(irq);
		NVIC_EnableIRQ(irq);
		NVIC_SetPriority(irq, priority);
	}
}

//割り込み不許可
static inline void DiseableInt(INT_ID id) {
	auto irq = GetIRQ(id);
	if (irq != WDT_IRQn) {
		NVIC_DisableIRQ(irq);
	}
}

//割り込みを書くときの見本
extern "C" void PIN_INT0_IRQHandler(void) {
	auto id = INT_ID::INT0;
	auto& handler = handlers[(uint32_t) id];
	if (handler != nullptr) handler();
	ClrIntSta(id);
}

extern "C" void PIN_INT1_IRQHandler(void) {
	auto id = INT_ID::INT1;
	auto& handler = handlers[(uint32_t) id];
	if (handler != nullptr) handler();
	ClrIntSta(id);
}

extern "C" void PIN_INT2_IRQHandler(void) {
	auto id = INT_ID::INT2;
	auto& handler = handlers[(uint32_t) id];
	if (handler != nullptr) handler();
	ClrIntSta(id);
}

extern "C" void PIN_INT3_IRQHandler(void) {
	auto id = INT_ID::INT3;
	auto& handler = handlers[(uint32_t) id];
	if (handler != nullptr) handler();
	ClrIntSta(id);
}

extern "C" void PIN_INT4_IRQHandler(void) {
	auto id = INT_ID::INT4;
	auto& handler = handlers[(uint32_t) id];
	if (handler != nullptr) handler();
	ClrIntSta(id);
}

extern "C" void PIN_INT5_IRQHandler(void) {
	auto id = INT_ID::INT5;
	auto& handler = handlers[(uint32_t) id];
	if (handler != nullptr) handler();
	ClrIntSta(id);
}

extern "C" void PIN_INT6_IRQHandler(void) {
	auto id = INT_ID::INT6;
	auto& handler = handlers[(uint32_t) id];
	if (handler != nullptr) handler();
	ClrIntSta(id);
}

extern "C" void PIN_INT7_IRQHandler(void) {
	auto id = INT_ID::INT7;
	auto& handler = handlers[(uint32_t) id];
	if (handler != nullptr) handler();
	ClrIntSta(id);
}

}
}
