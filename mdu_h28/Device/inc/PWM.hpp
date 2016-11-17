/*
 * PWM.h
 *
 *  Created on: 2016/11/04
 *      Author: TERU
 */

#ifndef PWM_H_
#define PWM_H_

#include <type.hpp>

namespace Device {

namespace PWM {

//DRVのデータシートを参照のこと。
enum class Signal
	:uint32_t {
	AB = 0b0110,
	AB_CB = 0b0101,
	CB = 0b0100,
	CB_CA = 0b1101,
	CA = 0b1100,
	CA_BA = 0b1001,
	BA = 0b1000,
	BA_BC = 0b1011,
	BC = 0b1010,
	BC_AC = 0b0011,
	AC = 0b0010,
	AC_AB = 0b0111,
	Align = 0b1110,
	Stop = 0b0000,
	Halt = Stop
};


void Init();
void SetDutyRaw(uint32_t);
void SetDuty(common::q32_t duty);
uint32_t GetCycle();
void Clear(); //すべてのPWM出力をなくす

void SetSignal(Signal);



}

} /* namespace Device */

#endif /* PWM_H_ */
