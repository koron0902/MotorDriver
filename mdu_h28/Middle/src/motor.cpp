#include <Motor.hpp>
#include <MotorSub.hpp>
#include "DRV.hpp"
#include <XPort.hpp>

using namespace common;
using namespace Device;

namespace Middle {
namespace Motor {

static IMotor* action = nullptr;

volatile void Wait() {
	for (volatile int i = 0; i < 180000; i++)
		;
}

//このファイルはモーターの種類を抽象化することを目的とする。
void Init() {
	//ICを1PWMモードに変更する
	DRV::Init();
	//制御法を設定する。
	SwitchMotorType(MotorType::DCMotor);
}

void SwitchMotorType(Motor::MotorType type) {
	//すでに実体化していないか確認
	if (action!=nullptr){
		if (action->GetType()==type){
			XPort::WriteLine("No Switch");
			return;
		}else{
			XPort::WriteLine("Release");
			delete action;
		}
	}
	//次の状態へ移行
	switch (type){
	case MotorType::DCMotor:
		action =new DCMotor();
		XPort::WriteLine("Switch to DCMotor");
		break;
	case MotorType::BLDCWithSensor:
		action=new BLDCMotorWithSensor();
		XPort::WriteLine("Switch to BLDC With Sensor");
		break;
	case MotorType::None:
	default:
		XPort::WriteLine("Switch to None");
	}
}
MotorType GetMotorType(){
	if (action!=nullptr){
		return action->GetType();
	}else{
		return MotorType::None;
	}
}

constexpr const char* const GetName(MotorType t){
	switch (t){
	case MotorType::None:
		return "None";
	case MotorType::DCMotor:
		return "DC-Motor";
	case MotorType::BLDCWithSensor:
		return "BLDC With Sensor";
	default:
		return "Others";
	}
}

void SetDuty(fix32 fix) {
	if (action != nullptr) {
		action->SetDuty(fix);
	}
}

void Lock() {
	if (action != nullptr) {
		action->Lock();
	}
}

void Free() {
	if (action != nullptr) {
		action->Free();
	}
}

}
}
