#include <chip.hpp>
#include <helper.hpp>
#include <QEI.hpp>

#define __USED __attribute__((used))

using namespace std;
using namespace common;

namespace Device {
namespace QEI {

union QEI_INT {
	uint32_t Word;
	struct {
		unsigned INV :1;
		unsigned TIM :1;
		unsigned VELC :1;
		unsigned DIR :1;

		unsigned ERR :1;
		unsigned ENCLK :1;
		unsigned POS0 :1;
		unsigned POS1 :1;

		unsigned POS2 :1;
		unsigned REV0 :1;
		unsigned POS0REV :1;
		unsigned POS1REV :1;

		unsigned POS2REV :1;
		unsigned REV1 :1;
		unsigned REV2 :1;
		unsigned MAXPOS :1;

		unsigned :16;
	};
};

//なぜかQEIライブラリが存在しないので構造体を定義
struct QEI_T {
	//Control Register
	//__O uint32_t CON; //
	union {
		__O uint32_t CON;
		struct {
			__O unsigned RESP :1;	//
			__O unsigned RESPI :1;	//
			__O unsigned RESV :1;	//
			__O unsigned RESI :1;	//
			__O unsigned :24;	//
		};
	};
	union {
		__I uint32_t STAT;
		struct {
			__I unsigned DIR :1;	//
			__I unsigned :31;	//
		};
	};
	union {
		__IO uint32_t CONF;
		struct {
			__IO unsigned DIRINV :1;	//
			__IO unsigned SIGMODE :1;	//
			__IO unsigned CAPMODE :1;	//
			__IO unsigned INVINX :1;	//
			__IO unsigned CRESPI :1;	//
			__IO unsigned :10;	//
			__IO unsigned INXGATE :4;	//
		};
	};
	//Status Register
	__I uint32_t POS;	//
	__IO uint32_t MAXPOS;	//
	union{
		__IO uint32_t CMPOS[3];
		struct{
			__IO uint32_t CMPOS0;	//
			__IO uint32_t CMPOS1;	//
			__IO uint32_t CMPOS2;	//
		};
	};
	__I uint32_t INXCNT;	//
	__IO uint32_t INXCMP0;	//
	__IO uint32_t LOAD;	//
	__I uint32_t TIME;	//
	__I uint32_t VEL;	//
	__I uint32_t CAP;	//
	__IO uint32_t VELCOMP;	//
	__IO uint32_t FILTERPHA;	//
	__IO uint32_t FILTERPHB;	//
	__IO uint32_t FILTERINX;	//
	__IO uint32_t WINDOW;	//
	__IO uint32_t INXCMP1;	//
	__IO uint32_t INXCMP2;	//
	//Interrupt Register
	__O QEI_INT IEC;	//
	__O QEI_INT IES;	//
	__I QEI_INT INSTART;	//
	__I QEI_INT IE;	//
	__O QEI_INT CLR;	//
	__O QEI_INT SET;	//
};

static constexpr QEI_T* QEI = (QEI_T*) LPC_QEI_BASE;

static constexpr uint32_t FilterFrequency=1_MHz;//デジタルフィルタの周波数
static constexpr uint32_t MeasureFrequency=100_Hz;//速度を計測する周波数

static inline void Reset() {
	//Positionを初期化する
	SetFilter(FilterFrequency/SystemCoreClock);
	SetTimer(MeasureFrequency/SystemCoreClock);
	QEI->CON = 0b1111;

}

void Init() {
	//clock supply
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_QEI);
	Reset();
	QEI->CAPMODE = true;	//x4 mode

}

void DirInv(bool dir ) {
	QEI->DIRINV = dir;
}

bool GetDirection() {
	return QEI->DIR;
}

void SetFilter(uint32_t clock) {
	QEI->FILTERINX = QEI->FILTERPHA = QEI->FILTERPHB = clock;
}

void SetPosition(uint32_t value){
	QEI->POS=value;
}

uint32_t GetPosition(){
	return QEI->POS;
}

uint32_t GetIndex(){
	return QEI->INXCNT;
}

void SetTimer(uint32_t clock){
	QEI->LOAD=clock;
}

uint32_t GetVelcoity(){
	return QEI->VEL;
}

}
}

