/*
 * motor.h
 *
 *  Created on: 2016/11/12
 *      Author: hamus
 */

#ifndef MIDDLE_MOTOR_H_
#define MIDDLE_MOTOR_H_

#include <qmath.hpp>
#include <stdint.h>

namespace Middle {
namespace Motor{

//このファイルはモーターの種類を抽象化することを目的とする。

void Init();

enum class MotorType:uint32_t {
	None=0,
	DCMotor=1,
	BLDCWithSensor=2,		//ホールセンサで駆動するモード
	BLDCWithoutSensor=3,	//コンパレータで
	BLDCWtihAmp=4,			//電流センサーで駆動する
	Error,//末尾
};



void SetDuty(common::fix32);
void Lock();
void Free();

void SwitchMotorType(MotorType);
MotorType GetMotorType();
const char* const GetName(MotorType);

static inline const char * const GetMotorTypeName(){
	return GetName(GetMotorType());
}



}
}

#endif /* MIDDLE_MOTOR_H_ */
