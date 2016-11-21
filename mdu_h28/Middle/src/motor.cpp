#include <motor.hpp>
#include <PWM.hpp>
#include "DRV.hpp"
#include <functional>
#include <utilize.hpp>
#include <qmath.hpp>
using namespace common;
using namespace Device;

namespace Middle {
namespace Motor {

static IMotor* action = nullptr;

volatile void Wait(){
	for (volatile int i = 0;i < 180000; i++);
}

//このファイルはモーターの種類を抽象化することを目的とする。
void Init() {
	//ICを1PWMモードに変更する
	DRV::Init();
	//制御法を設定する。
	//ModeAs(Type::None);

	ModeAs(Type::DCMotor);
	for(int i = 0;i < 4;i++){
		SetDuty(fix32::CreateFloat(0.1 * i));
		Wait();
	}
}

void ModeAs(Type t) {
	Release(action);
	switch (t) {
	case Type::DCMotor:
		action = new DCMotor();
		break;
	default:
		action = nullptr;
		break;
	}
}

void SetDuty(fix32 fix){
	if(action!=nullptr){
		action->SetDuty(fix);
	}
}

void Lock() {
	if (action != nullptr) {
		action->Lock();
	}
}

void Free() {
	if (action!=nullptr){
		action->Free();
	}
}

DCMotor::DCMotor() {
	Free();
}

DCMotor::~DCMotor() {
	Free();
}

void DCMotor::Free() {
	using namespace PWM;
	SetDutyRaw(0);
	SetSignal(Signal::Halt);
}

void DCMotor::Lock() {
	using namespace PWM;
	SetSignal(Signal::CB_CA);
	SetDutyRaw(0);
}

void DCMotor::SetDuty(fix32 duty) {
	using namespace PWM;
	using namespace common;
	constexpr uint32_t eps = 5;
	fix32 c = regions::one.Fit(duty);
	fix32 a = abs(c);
	if(a == fix32::One)
		a -= fix32::CreateRaw(eps);
	q32_t q = a.GetRaw() << 16; //小数部のみにする。
	bool s = sign(c);

	if (s) {
		//正転
		SetSignal(Signal::AB);
	} else {
		//逆転
		SetSignal(Signal::BA);
	}
	PWM::SetDuty(q);
}

}
}
