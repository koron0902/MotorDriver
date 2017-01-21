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

enum class MotorType:uint32_t {
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

void SetDuty(common::fix32);
void Lock();
void Free();

void SwitchMotorType(MotorType);


class IMotor{
protected:
	MotorType type;//識別用
public:
	IMotor(MotorType t):type(t){}
	IMotor(const IMotor&)=default;
	virtual ~IMotor()=default;

	virtual void SetDuty(common::fix32)=0;
	virtual void Lock()=0;
	virtual void Free()=0;

	MotorType GetType()const{return type;}
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

//ホールセンサーから次の状態を出力するクラス
class HoleStateGenerator{
	bool direction;
public:
	HoleStateGenerator(bool dir=false):
		direction(dir){}
	void operator()(HoleSensor::HoleStatus sta);
	void SetDirection(bool dir){direction=dir;}
};

class BLDCMotorWithSensor: public IMotor{
	Mode mMode;
	HoleStateGenerator state;
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
