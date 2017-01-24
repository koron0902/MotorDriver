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
enum class Pulse
	:uint32_t {
	UV = 0b0110,
	AB_CB = 0b0101,
	WV = 0b0100,
	CB_CA = 0b1101,
	WU = 0b1100,
	CA_BA = 0b1001,
	VU = 0b1000,
	BA_BC = 0b1011,
	VW = 0b1010,
	BC_AC = 0b0011,
	UW = 0b0010,
	AC_AB = 0b0111,
	Align = 0b1110,
	Stop = 0b0000,
	Halt = Stop,
	None= Stop	//HoleStatusとの互換を持たせるため
};


void Init();
void SetDutyRaw(uint32_t);
void SetDuty(common::q32_t duty);
uint32_t GetCycle();
void Clear(); //すべてのPWM出力をなくす

void SetSignal(Pulse);
void SetHandler(const common::callback_t&);


Pulse InvertSignal(Pulse);



}

} /* namespace Device */

#endif /* PWM_H_ */
