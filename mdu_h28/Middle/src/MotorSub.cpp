#include <MotorSub.hpp>
#include <PWM.hpp>
#include <region.hpp>
namespace Middle {
namespace Motor {

using namespace common;
using namespace Device;

static HoleStateGenerator state;

DCMotor::DCMotor() :
		IMotor(MotorType::DCMotor) {
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
	enum class Mode {
		Free, Lock, Running
	};
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

static inline constexpr PWM::Pulse next(HoleSensor::HoleStatus data) {
	using namespace HoleSensor;
	using namespace PWM;



	switch (data) {
	case HoleStatus::U:
		return Pulse::AC;
	case HoleStatus::W:
		return Pulse::BC;
	case HoleStatus::UW:
		return Pulse::BA;
	case HoleStatus::V:
		return Pulse::AB;
	case HoleStatus::VU:
		return Pulse::CB;
	case HoleStatus::VW:
		return Pulse::BC;
	case HoleStatus::None:
	default:
		return PWM::Pulse::None;
	}
}

void HoleStateGenerator::operator ()(HoleSensor::HoleStatus sta) {
	if (direction) {
		PWM::SetSignal(next(sta));
	} else {
		PWM::SetSignal(previous(sta));
	}
}

BLDCMotorWithSensor::BLDCMotorWithSensor() :
		IMotor(MotorType::BLDCWithSensor) {
	HoleSensor::SetHandler([](HoleSensor::HoleStatus data){
		state.operator ()(data);
	});
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
	auto s = duty.GetRaw()<0;
	// モータ始動用に現在の位置から出力を決定する。
	state.SetDirection(s);
	state.operator ()(HoleSensor::GetState());
	PWM::SetDuty(q);
}

}
}
