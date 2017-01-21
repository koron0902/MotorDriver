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
		static Motor::Type mType;
		volatile void Wait(){
			for (volatile int i = 0;i < 180000; i++);
		}

		//このファイルはモーターの種類を抽象化することを目的とする。
		void Init() {
			//ICを1PWMモードに変更する
			DRV::Init();
			//制御法を設定する。
			SwitchMotorType(Type::DCMotor);
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

		void SwitchMotorType(Motor::Type _type){
			if(mType == _type)
				XPort::WriteLine( "Already set as requested type");

			Release(action);

			switch(_type){
			case Type::DCMotor:
				action = new DCMotor();
				XPort::WriteLine( "Succeeded in switching DC Motor");
				break;
			case Type::BLDCWithSensor:
				action = new BLDCMotorWithSensor();
				XPort::WriteLine( "Succeeded in switching BLDC Motor(No Encoder)");
				break;
			default:
				action = nullptr;
				XPort::WriteLine("Released Motor");
				break;
			}
			mType = _type;
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
			constexpr uint32_t eps = 100;
			fix32 c = regions::one.Fit(duty);
			fix32 a = abs(c);
			if(a == fix32::One)
				a -= fix32::CreateRaw(eps);
			q32_t q = a.GetRaw() << 16; //小数部のみにする。
			bool s = sign(c);

			if (!s) {
				//正転
				SetSignal(Signal::AB);
			} else {
				//逆転
				SetSignal(Signal::BA);
			}
			PWM::SetDuty(q);
		}

		BLDCMotorWithSensor::BLDCMotorWithSensor(){
			direction = false;
			HallSensorCallBack = [](HoleSensor::HoleStatus data, bool direction)->void{
				using namespace HoleSensor;
				PWM::Signal nextSignal = PWM::Signal::AB;
				switch(data){
				/*
				 * 参考URL
				 * https://toshiba.semicon-storage.com/jp/design-support/e-learning/brushless_motor/chap4/1274636.html
				 */
					case HoleStatus::U:
						nextSignal = PWM::Signal::CB;
						break;
					case HoleStatus::UV:
						nextSignal = PWM::Signal::CA;
						break;
					case HoleStatus::V:
						nextSignal = PWM::Signal::BA;
						break;
					case HoleStatus::VW:
						nextSignal = PWM::Signal::BC;
						break;
					case HoleStatus::W:
						nextSignal = PWM::Signal::AC;
						break;
					case HoleStatus::WU:
						nextSignal = PWM::Signal::AB;
						break;
					case HoleStatus::None:
						break;
					default:
						break;
				}
				if(direction)
					PWM::SetSignal(PWM::InvertSignal(nextSignal));
				else
					PWM::SetSignal(nextSignal);
			};
			HoleSensor::SetHandler(HallSensorCallBack);
			Free();
		}

		BLDCMotorWithSensor::~BLDCMotorWithSensor(){
			Free();
		}

		void BLDCMotorWithSensor::Free(){
			using namespace PWM;
			SetDutyRaw(0);
			SetSignal(Signal::Halt);
		}

		void BLDCMotorWithSensor::Lock(){
			using namespace PWM;
			SetDutyRaw(0);
			SetSignal(Signal::CB_CA);
		}

		void BLDCMotorWithSensor::SetDuty(fix32 duty){
			using namespace PWM;
			using namespace common;
			constexpr uint32_t eps = 100;

			auto c = regions::one.Fit(duty);
			auto a = abs(c);
			if(a == fix32::One)
				a -= fix32::CreateRaw(eps);
			q32_t q = a.GetRaw() << 16;
			auto s = sign(c);

			HoleSensor::SetDirection(!s);
			// モータ始動用に現在の位置から出力を決定する。
			uint32_t data=0;
			data |= Port::HoleU.Get()?0b001:0;
			data |= Port::HoleV.Get()?0b010:0;
			data |= Port::HoleW.Get()?0b100:0;
			HallSensorCallBack((HoleSensor::HoleStatus)data, !s);

			PWM::SetDuty(q);
		}
	}
}
