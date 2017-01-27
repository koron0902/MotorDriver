#include <chip.hpp>
#include <QEI.hpp>
#include <QEISub.hpp>
#include <configuration.hpp>
#include <Port.hpp>
#include <bits.hpp>
#include <Timer.hpp>
#define __USED __attribute__((used))

using namespace std;
using namespace common;

namespace Device {
namespace QEI {

static QEI_T* QEI = (QEI_T*) LPC_QEI_BASE;
static callback_t Handler;
static void SetTimer(uint32_t clock);

static int64_t position; //速度の積分と思ってほしい。
static int64_t velcoity;

static void WaitForZero(volatile uint32_t* reg) {
	while (*reg != 0)
		;
}

static inline void Reset() {
	QEI->RESP = true;
	QEI->RESPI = true;
	QEI->RESV = true;
	QEI->RESI = true;
}

void QEI_Handler() {
	bool dir = QEI->DIR;
	int32_t value = QEI->POS;

	if (dir) {
		value=-(~value+1);
	}
	position += value;
	velcoity = value;

	QEI->RESP = true;
}

void Init() {
	//clock supply
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_QEI);
	Chip_SYSCTL_PeriphReset(RESET_QEI0);
	Reset();
	QEI->MAXPOS = UINT32_MAX;
	QEI->SIGMODE = 0;
	//QEI->LOAD = 0xFFFFFFFF - ((0xFFFFFFFF) * (1.0 * 30_KHz / SystemCoreClock));
	SetTimer(CycleControlDefault);
	//QEI->FILTERINX = QEI->FILTERPHA = QEI->FILTERPHB = 500;
	SetFilter(0);
	QEI->CAPMODE = true;
	QEI->CON = 0b1111;

	position = 0;
	velcoity = 0;

	Timer::SetHandler(QEI_Handler, Timer::TimerID::Timer3,
			SystemCoreClock / CycleControlDefault);
	//Interrupt Setting
	/*
	 NVIC_DisableIRQ(QEI_IRQn);
	 QEI->IE.DIR = true;
	 QEI->IE.MAXPOS=true;

	 Handler = nullptr;
	 NVIC_SetPriority(QEI_IRQn, PriorityQEI);
	 NVIC_EnableIRQ(QEI_IRQn);
	 */
}

static void SetTimer(uint32_t clock) {
	QEI->LOAD = (SystemCoreClock / clock) + 1;
}

void SetFilter(uint32_t clock) {
	QEI->FILTERINX = QEI->FILTERPHA = QEI->FILTERPHB = clock;
}

bool GetDirection() {
	return QEI->DIR;
}

int64_t GetPosition() {
	return position;
}
int32_t GetPositionRaw() {
	return GetDirection() ? QEI->POS : -QEI->POS;
}

int32_t GetVelcoity() {
	return velcoity;
}

std::string GetPulseName() {
	constexpr auto mark = [](bool f)->char {
		return f?'1':'0';
	};

	bool x = Port::QEI_X.Get();
	bool y = Port::QEI_Y.Get();
	//bool z=Port::QEI_Z.Get();
	string s;
	s += mark(x);
	s += mark(y);
	//s+=mark(z);
	return s;
}
/*
 void SetHandler(const callback_t& func, uint8_t Priority) {
 if (func != nullptr) {
 NVIC_DisableIRQ(QEI_IRQn);
 Handler = func;
 NVIC_SetPriority(QEI_IRQn, Priority);
 NVIC_EnableIRQ(QEI_IRQn);
 } else {
 NVIC_DisableIRQ(QEI_IRQn);
 }
 }
 */

}
}

