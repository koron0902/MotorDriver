#include <chip.hpp>
#include <QEI.hpp>
#include <QEISub.hpp>
#include <configuration.hpp>
#include <Port.hpp>
#define __USED __attribute__((used))

using namespace std;
using namespace common;

namespace Device {
namespace QEI {

static QEI_T* QEI = (QEI_T*) LPC_QEI_BASE;
static callback_t Handler = nullptr;
//static constexpr uint32_t FilterFrequency=1_MHz;//デジタルフィルタの周波数
//static constexpr uint32_t MeasureFrequency=100_Hz;//速度を計測する周波数
volatile uint32_t* QEIVel;

void WaitForZero(volatile uint32_t* reg) {
	while (*reg != 0)
		;
}

void Init() {
	//clock supply
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_QEI);
	Chip_SYSCTL_PeriphReset(RESET_QEI0);
	QEI->MAXPOS = 40;
	QEI->SIGMODE = 0;
	//QEI->LOAD = 0xFFFFFFFF - ((0xFFFFFFFF) * (1.0 * 30_KHz / SystemCoreClock));
	SetTimer(CycleControl);
	//QEI->FILTERINX = QEI->FILTERPHA = QEI->FILTERPHB = 500;
	SetFilter(0);
	QEI->CAPMODE = true;
	QEI->CON = 0b1111;
	//while(QEI->CAP != 0);
	WaitForZero(&(QEI->CAP));
	QEIVel = &(QEI->CAP);
}

void SetTimer(uint32_t clock) {
	QEI->LOAD = (/*(double)*/SystemCoreClock / clock) + 1;
}

void SetFilter(uint32_t clock) {
	QEI->FILTERINX = QEI->FILTERPHA = QEI->FILTERPHB = clock;
}

uint32_t GetPosition(){
	return QEI->POS;
}

uint32_t GetVelcoity() {
	return QEI->CAP;
}

std::string GetPulseName(){
	constexpr auto mark=[](bool f)->char{
		return f?'1':'0';
	};

	bool x=Port::QEI_X.Get();
	bool y=Port::QEI_Y.Get();
	//bool z=Port::QEI_Z.Get();
	string s;
	s+=mark(x);
	s+=mark(y);
	//s+=mark(z);
	return s;
}

void SetHandler(const callback_t& func, uint8_t Priority) {
	if (func != nullptr) {
		NVIC_DisableIRQ(QEI_IRQn);
		Handler=func;
		NVIC_SetPriority(QEI_IRQn, Priority);
		NVIC_EnableIRQ(QEI_IRQn);
	} else {
		NVIC_DisableIRQ(QEI_IRQn);
	}
}

extern "C"
void  QEI_IRQHandler(){
	if (Handler!=nullptr){
		Handler();
	}else{
		//呼ばれることがすでにおかしい
		NVIC_DisableIRQ(QEI_IRQn);
	}
}


}
}

