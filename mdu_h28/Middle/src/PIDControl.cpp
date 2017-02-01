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
#include <text.hpp>

namespace Middle{
	namespace Controller{
	fix32 PID::LastDuty;
	PID::MotorState_t PID::mNextState, PID::mLastState;
	float PID::mGearRate, PID::mRadius, PID::mEncoderResolution, PID::mKp, PID::mKi, PID::mKd, PID::mKe;
		PID::PID(){
			mGearRate = GEAR_RATE_DEFAULT;
			mRadius = RADIUS_DEFAULT;
			mEncoderResolution = ENCODER_RESOLUTION_DEFAULT;
			mKp = K_P_DEFAULT;
			mKi = K_I_DEFAULT;
			mKd = K_D_DEFAULT;
			mKe = K_E_DEFAULT;
			CallProc = [this](){
				this->Proc(mLastState, mNextState);
				mLastState = mNextState;
			};
		}

		PID::~PID(){
		}

		void PID::Proc(MotorState_t& last, MotorState_t& next){
			float gains[] = {mKp, mKi, mKd, mKe};
			Matrix<float, 1, 4> GainMatrix(gains); // フィードバックゲイン行列
			static Matrix<float, 4, 1> in_vector;	// 偏差等格納行列


			auto BatteryVoltage = (float)Device::ADC::GetVolt() * 100;	// 電源電圧
			auto Volt2Duty = 4096 * 6.25f / (BatteryVoltage / 16);		// 出力->duty変換係数(4096倍)
			auto Pulse2RPS = (float)mFreq / mEncoderResolution;			// パルス->速度変換係数

			// 観測パルス数よりモータの回転速度を算出
			next.mRealSpeed = -Device::QEI::GetPulseCount() * Pulse2RPS;

			//next.mTargetSpeed = 30;//next.mRealSpeed;
			auto error = next.mTargetSpeed - next.mRealSpeed;
			auto integ = last.mIntegration + error;
			static constexpr auto min = -150.0f ;//<< fix32::shift;
			static constexpr auto max = 150.0f;// << fix32::shift;
			integ = (integ < min ? min : (integ > max ? max : integ));

			in_vector(0, 0) = error;
			in_vector(1, 0) = integ;
			in_vector(2, 0) = error - last.mError;
			in_vector(3, 0) = next.mRealSpeed;

			// ゲイン行列と偏差・積分値等の行列の内積
			auto output = GainMatrix * in_vector;

			output(0, 0) *= Volt2Duty;
			auto duty = output(0, 0);
			duty /= 4096;
			duty = (duty < -1 ? -1 : (duty > 1 ? 1 : duty));

			next.mIntegration = integ;
			next.mError = error;
			next.mDuty = duty;

			Middle::Motor::SetDuty(regions::one.Fit(fix32::CreateFloat(duty)));
		}

		void PID::SetFreq(fix32 freq){
			mFreq = freq;
			//Device::Timer::SetAction(1, mFreq.word, this);
		}

		void PID::SetFreq(uint32_t freq){
			mFreq = fix32::CreateInt(freq);
			//Device::Timer::SetAction(1, mFreq.word, this);
		}


		void PID::SetGainP(float p){
			mKp = p;
		}

		void PID::SetGainI(float i){
			mKi = i;
		}

		void PID::SetGainD(float d){
			mKd = d;
		}
		void PID::SetGearRate(float g){
			mGearRate = g;
		}

		void PID::SetTargetSpeed(float speed){
			mNextState.mTargetSpeed = speed;
		}

		std::string PID::GetGainP(){
			return common::ToStrF(PID::mKp);
		}

		std::string PID::GetGainI(){
			return common::ToStrF(PID::mKi);
		}
		std::string PID::GetGainD(){
			return common::ToStrF(PID::mKd);
		}
		std::string PID::GetGearRate(){
			return common::ToStrF(PID::mGearRate);
		}
		std::string PID::GetSpeed(){
			return common::ToStrF(PID::mLastState.mRealSpeed);
		}
	}
}
