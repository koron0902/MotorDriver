/*
 * PIDControl.cpp
 *
 *  Created on: 2016/11/21
 *      Author: Keita
 */

#include <PIDControl.hpp>
#include <matrix.hpp>
#include <Port.hpp>
#include <QEI.hpp>
#include <ADC.hpp>
#include <PWM.hpp>
#include <motor.hpp>
#include <region.hpp>

namespace Middle{
	namespace Controller{
	fix32 PID::LastDuty;
		PID::PID()://mFreq(FREQUENCY_DEFAULT),
				mGearRate(GEAR_RATE_DEFAULT),
				mRadius(RADIUS_DEFAULT),
				mEncoderResolution(ENCODER_RESOLUTION_DEFAULT),
				mKp(K_P_DEFAULT),
				mKi(K_I_DEFAULT),
				mKd(K_D_DEFAULT),
				mKe(K_E_DEFAULT){
			CallProc = [this](){
				this->Proc(mLastState, mNextState);
			};

			Device::Port::Set(Device::Port::PWMEN, true);
			Middle::Motor::ModeAs(Middle::Motor::Type::DCMotor);
		}

		PID::~PID(){
			//Device::Timer::SetAction(1, mFreq.word, nullptr);
			//Middle::Motor::ModeAs(Middle::Motor::Type::None);
			//Device::Port::Set(Device::Port::PWMEN, false);
		}

		void PID::Proc(MotorState_t& last, MotorState_t& next){
			static const float gains[] = {mKp, mKi, mKd, mKe};
			static const Matrix<float, 1, 4> GainMatrix(gains);
			//for(int i = 0;i < 4;i++)
				//GainMatrix(0, i) = gains[i];
			static Matrix<float, 4, 1> in_vector;


			static const auto BatteryVoltage = (float)Device::ADC::GetVolt() * 100;
			static const auto Volt2Duty = 4096 * 6.25f / (BatteryVoltage / 16);
			static const auto Pulse2RPS = (float)mFreq / mEncoderResolution;

			next.mRealSpeed = Device::QEI::GetVelcoity() * Pulse2RPS;

			next.mTargetSpeed = next.mRealSpeed;
			static const auto error = next.mTargetSpeed - next.mRealSpeed;
			static auto integ = last.mIntegration + error;
			static constexpr auto min = -10.0f ;//<< fix32::shift;
			static constexpr auto max = 10.0f;// << fix32::shift;
			integ = (integ < min ? min : (integ > max ? max : integ));

			in_vector(0, 0) = error;
			in_vector(1, 0) = integ;
			in_vector(2, 0) = error - last.mError;
			in_vector(3, 0) = next.mRealSpeed;

			auto output = GainMatrix * in_vector;

			output(0, 0) *= Volt2Duty;
			auto duty = output(0, 0);
			duty /= 4096;
			duty = (duty < -1 ? -1 : (duty > 1 ? 1 : duty));

			next.mIntegration = integ;
			next.mError = error;
			next.mDuty = duty;

			Middle::Motor::SetDuty(regions::one.Fit(fix32::CreateFloat(duty)));
			last = next;
			LastDuty = last.mDuty;
		}

		void PID::SetFreq(fix32 freq){
			mFreq = freq;
			//Device::Timer::SetAction(1, mFreq.word, this);
		}

		void PID::SetFreq(uint32_t freq){
			mFreq = fix32::CreateInt(freq);
			//Device::Timer::SetAction(1, mFreq.word, this);
		}

		void PID::SetGainP(fix32 p){
			mKp = p;
		}

		void PID::SetGainP(float p){
			mKp = fix32::CreateFloat(p);
		}

		void PID::SetGainI(fix32 i){
			mKi = i;
		}

		void PID::SetGainI(float i){
			mKi = fix32::CreateFloat(i);
		}

		void PID::SetGainD(fix32 d){
			mKd = d;
		}

		void PID::SetGainD(float d){
			mKd = fix32::CreateFloat(d);
		}

		void PID::SetGearRate(fix32 g){
			mGearRate = g;
		}

		void PID::SetGearRate(uint32_t g){
			mGearRate = fix32::CreateInt(g);
		}

		void PID::SetTargetSpeed(fix32 speed){
			mNextState.mTargetSpeed = speed;
		}

		void PID::SetTargetSpeed(float speed){
			mNextState.mTargetSpeed = fix32::CreateFloat(speed);
		}
	}
}
