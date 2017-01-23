#include <Motor.hpp>
#include <PWM.hpp>
#include "DRV.hpp"
#include <functional>
#include <utilize.hpp>
#include <qmath.hpp>
#include <region.hpp>
#include <Port.hpp>
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

DCMotor::DCMotor() :IMotor(MotorType::DCMotor){
	Free();
}

DCMotor::~DCMotor() {
	Free();
}

void DCMotor::Free() {
	using namespace PWM;
	SetDutyRaw(0);
	SetSignal(Pulse::Halt);
}

void DCMotor::Lock() {
	using namespace PWM;
	SetSignal(Pulse::CB_CA);
	SetDutyRaw(0);
}

void DCMotor::SetDuty(fix32 duty) {
	using namespace PWM;
	using namespace common;
	constexpr uint32_t eps = 100;
	fix32 c = regions::one.Fit(duty);
	fix32 a = abs(c);
	if (a == fix32::One) a -= fix32::CreateRaw(eps);
	q32_t q = a.GetRaw() << 16; //小数部のみにする。
	bool s = sign(c);

	if (!s) {
		//正転
		SetSignal(Pulse::AB);
	} else {
		//逆転
		SetSignal(Pulse::BA);
	}
	PWM::SetDuty(q);
}

static inline constexpr PWM::Pulse previous(HoleSensor::HoleStatus data) {
	using namespace HoleSensor;
	switch (data) {
	case HoleStatus::U:
		return PWM::Pulse::CB;
	case HoleStatus::UV:
		return PWM::Pulse::CA;
	case HoleStatus::V:
		return PWM::Pulse::BA;
	case HoleStatus::VW:
		return PWM::Pulse::BC;
	case HoleStatus::W:
		return PWM::Pulse::AC;
	case HoleStatus::WU:
		return PWM::Pulse::AB;
	case HoleStatus::None:
	default:
		return PWM::Pulse::None;
	}
}

static inline constexpr PWM::Pulse next(HoleSensor::HoleStatus data){
	using namespace HoleSensor;
	switch (data) {
	case HoleStatus::U:
		return PWM::Pulse::BC;
	case HoleStatus::UV:
		return PWM::Pulse::AC;
	case HoleStatus::V:
		return PWM::Pulse::AB;
	case HoleStatus::VW:
		return PWM::Pulse::CB;
	case HoleStatus::W:
		return PWM::Pulse::CA;
	case HoleStatus::WU:
		return PWM::Pulse::BA;
	case HoleStatus::None:
	default:
		return PWM::Pulse::None;
	}
}

void HoleStateGenerator::operator ()(HoleSensor::HoleStatus sta){
	if (direction){
			PWM::SetSignal(next(sta));
		}else{
			PWM::SetSignal(previous(sta));
		}
}

BLDCMotorWithSensor::BLDCMotorWithSensor() :IMotor(MotorType::BLDCWithSensor){
	HoleSensor::SetHandler(state);
	Free();
}

BLDCMotorWithSensor::~BLDCMotorWithSensor() {
	HoleSensor::SetHandler(nullptr);
	Free();
}

void BLDCMotorWithSensor::Free() {
	using namespace PWM;
	SetDutyRaw(0);
	SetSignal(Pulse::Halt);
}

void BLDCMotorWithSensor::Lock() {
	using namespace PWM;
	SetDutyRaw(0);
	SetSignal(Pulse::CB_CA);
}

void BLDCMotorWithSensor::SetDuty(fix32 duty) {
	using namespace PWM;
	using namespace common;
	constexpr uint32_t eps = 100;

	auto c = regions::one.Fit(duty);
	auto a = abs(c);
	if (a == fix32::One) a -= fix32::CreateRaw(eps);
	q32_t q = a.GetRaw() << 16;
	auto s = sign(c);
	// モータ始動用に現在の位置から出力を決定する。
	state.SetDirection(s);
	state.operator ()(HoleSensor::GetState());
	PWM::SetDuty(q);
}
}
}
