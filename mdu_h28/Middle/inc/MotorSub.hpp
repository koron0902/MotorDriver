#ifndef MIDDLE_INC_MOTORSUB_HPP_
#define MIDDLE_INC_MOTORSUB_HPP_
#include <HoleSensor.hpp>
#include <Motor.hpp>
namespace Middle{
namespace Motor{
using namespace Device;
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
	enum class Mode{
		Free,
		Lock,
		Running
	} m_mode;
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
	HoleStateGenerator(const HoleStateGenerator&)=default;
	void operator()(HoleSensor::HoleStatus sta);
	void SetDirection(bool dir){direction=dir;}
};

class BLDCMotorWithSensor: public IMotor{
public:
	BLDCMotorWithSensor();
	BLDCMotorWithSensor(const BLDCMotorWithSensor&) = default;
	virtual ~BLDCMotorWithSensor();
	virtual void SetDuty(common::fix32);
	virtual void Lock();
	virtual void Free();
};
}

}



#endif /* MIDDLE_INC_MOTORSUB_HPP_ */
