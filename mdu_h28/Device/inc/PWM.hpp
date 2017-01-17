/*
 * PWM.h
 *
 *  Created on: 2016/11/04
 *      Author: TERU
 */

#ifndef PWM_H_
#define PWM_H_

#include <status.hpp>

namespace Device {

namespace PWM {




void Init();
void SetDutyRaw(uint32_t);
void SetDuty(common::q32_t duty);
uint32_t GetCycle();
void Clear(); //すべてのPWM出力をなくす

void SetPWMMode(common::MotorPosition);



}

} /* namespace Device */

#endif /* PWM_H_ */
