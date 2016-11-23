/*
 * PIDControl.cpp
 *
 *  Created on: 2016/11/21
 *      Author: Keita
 */

#include <PIDControl.hpp>
#include <matrix.hpp>
#include <qmath.hpp>
#include <Timer.hpp>
#include <Port.hpp>
#include <QEI.hpp>
#include <ADC.hpp>
#include <PWM.hpp>
#include <motor.hpp>
#include <region.hpp>

namespace Middle{
	namespace Controller{
		PID::PID():mFreq(FREQUENCY_DEFAULT),
				mGearRate(GEAR_RATE_DEFAULT),
				mRadius(RADIUS_DEFAULT),
				mEncoderResolution(ENCODER_RESOLUTION_DEFAULT),
				mKp(K_P_DEFAULT),
				mKi(K_I_DEFAULT),
				mKd(K_D_DEFAULT),
				mKe(K_E_DEFAULT){
			Device::Port::Set(Device::Port::PWMEN, true);
			Middle::Motor::ModeAs(Middle::Motor::Type::DCMotor);
		}

		PID::~PID(){
			//Device::Timer::SetAction(1, mFreq.word, nullptr);
			Device::Port::Set(Device::Port::PWMEN, false);
		}

		//void PID::Proc(){

			//Calclate(mLastState, mNextState);
		//}


		void PID::Proc(MotorState_t& last, MotorState_t& next){
			mNextState.mRealSpeed = fix32::CreateFloat(Device::QEI::GetVelcoity()) / mEncoderResolution;
			const fix32 gains[] = {mKp, mKi, mKd, mKe};
			const Matrix<fix32, 1, 4> GainMatrix(gains[0]);
			Matrix<fix32, 4, 1> in_vector;

			fix32 error, integ, duty;
			static const fix32 BatteryVoltage = fix32::CreateFloat(16.8f);//Device::ADC::GetVlot();
			static const fix32 Volt2Duty = fix32::One / BatteryVoltage;

			error = fix32::CreateInt(1);//next.mTargetSpeed - next.mRealSpeed;
			integ = last.mIntegration + error;
			static constexpr fix32 min = -150 << fix32::shift;
			static constexpr fix32 max = 150 << fix32::shift;
			integ = (integ < min ? min : (integ > max ? max : integ));

			in_vector(0, 0) = error;
			in_vector(1, 0) = integ;
			in_vector(2, 0) = error - last.mError;
			in_vector(3, 0) = next.mRealSpeed;

			auto output = GainMatrix * in_vector;

			output(0, 0) *= Volt2Duty;
			duty = output(0, 0);
			duty = regions::one.Fit(duty);

			next.mIntegration = integ;
			next.mError = error;
			next.mDuty = duty;

			Middle::Motor::SetDuty(duty);
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
