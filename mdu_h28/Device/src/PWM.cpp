/*
 * PWM.cpp
 *
 *  Created on: 2016/11/04
 *      Author: TERU
 */

#include <bits.hpp>
#include <chip.hpp>
#include <Port.hpp>
#include <PWM.hpp>
#include <unit.hpp>
#include <configuration.hpp>
using namespace common;

namespace Device {

namespace PWM {

static uint32_t Rate = 30000; //[Hz]動作周波数
//SCT内部のピン配置
const uint8_t AH_PIN = 5;
const uint8_t AL_PIN = 2;
const uint8_t BH_PIN = 1;
const uint8_t BL_PIN = 4;
const uint8_t CH_PIN = 2;
const uint8_t CL_PIN = 0;

//制御用
static uint32_t divide=2;//分周率
static uint32_t counter;
static callback_t handler;
//補助関数
static inline void interrupt_disable(){
	NVIC_DisableIRQ(SCT1_IRQn);
}

static inline void interrupt_enable(){
	interrupt_disable();
	NVIC_SetPriority(SCT1_IRQn,PriorityCycle);
	NVIC_EnableIRQ(SCT1_IRQn);
}

//以下、実装
void Init() {
	auto assign=[](uint8_t id){
		Chip_SCTPWM_SetOutPin(LPC_SCT1, id, id);
	};

	Chip_SCTPWM_Init(LPC_SCT1);
	Chip_SCTPWM_SetRate(LPC_SCT1, Rate); //divide
	//Assignment
	assign(AH_PIN);
	//assign(BH_PIN);
	//assign(CH_PIN);

	//Chip_SCTPWM_PercentageToTicks(LPC_SCT1,50);
	Clear();
	Chip_SCTPWM_Start(LPC_SCT1);

	//制御用
	counter=0;
	handler=nullptr;
}

void SetDutyRaw(uint32_t raw){
	Chip_SCTPWM_SetDutyCycle(LPC_SCT1,AH_PIN,raw);
}

void SetDuty(q32_t duty){
	const uint32_t cycle=Chip_SCTPWM_GetTicksPerCycle(LPC_SCT1);
	SetDutyRaw(((uint64_t)duty*cycle)>>32);
}

uint32_t GetCycle(){
	return Chip_SCTPWM_GetTicksPerCycle(LPC_SCT1);
}


void Clear(){
	SetDutyRaw(0);
	SetSignal(Signal::Halt);
}

void SetSignal(Signal mode){
	using namespace Port;
	using namespace common;
	uint32_t port=(uint32_t)mode;
	//テーブル式で出力する。
	Set(PWMAL,port&Bit(3));
	Set(PWMBH,port&Bit(2));
	Set(PWMBL,port&Bit(1));
	Set(PWMCH,port&Bit(0));
}



void SetHandler(const callback_t& back,uint32_t div){
	interrupt_disable();
	if (div!=0){
		handler=back;
		interrupt_enable();
	}else{
		//実行されなくなる。
		handler=nullptr;
	}
}
extern "C"
void SCT1_IRQHandler(void) {
	//毎周期呼ばれると仮定する。



}

Signal InvertSignal(Signal mode){
	Signal invSig;
	switch(mode){
		case Signal::AB:
			invSig = Signal::BA;
			break;
		case Signal::AC:
			invSig = Signal::CA;
			break;
		case Signal::BA:
			invSig = Signal::AB;
			break;
		case Signal::BC:
			invSig = Signal::CB;
			break;
		case Signal::CA:
			invSig = Signal::AC;
			break;
		case Signal::CB:
			invSig = Signal::BC;
			break;
		default:
			invSig = mode;
	}
	return invSig;
}


}
}
