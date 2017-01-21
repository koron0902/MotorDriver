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
#include <HoleSensor.hpp>

namespace Middle {
namespace Motor{

//このファイルはモーターの種類を抽象化することを目的とする。

void Init();

enum class Type:uint32_t {
	None=0,
	DCMotor=1,
	BLDCWithSensor=2,		//ホールセンサで駆動するモード
	BLDCWithoutSensor=3,	//コンパレータで
	BLDCWtihAmp=4			//電流センサーで駆動する
};

enum class Mode{
	Free,
	Lock,
	Running
};

void ModeAs(Type);

void SetDuty(common::fix32);
void Lock();
void Free();

void SwitchMotorType(Type);


class IMotor{
public:
	IMotor()=default;
	IMotor(const IMotor&)=default;
	virtual ~IMotor()=default;

	virtual void SetDuty(common::fix32)=0;
	virtual void Lock()=0;
	virtual void Free()=0;
};

class DCMotor:public IMotor{
	// WV相で動作する。
	Mode m_mode;
public :
	DCMotor ();
	DCMotor(const DCMotor&)=default;
	virtual ~DCMotor();
	virtual void SetDuty(common::fix32);
	virtual void Lock();
	virtual void Free();
};

class BLDCMotorWithSensor: public IMotor{
	Mode mMode;
	std::function<void(HoleSensor::HoleStatus, bool)> HallSensorCallBack;
public:
	bool direction;
	BLDCMotorWithSensor();
	BLDCMotorWithSensor(const BLDCMotorWithSensor&) = default;
	virtual ~BLDCMotorWithSensor();
	virtual void SetDuty(common::fix32);
	virtual void Lock();
	virtual void Free();
};
}

}

#endif /* MIDDLE_MOTOR_H_ */
