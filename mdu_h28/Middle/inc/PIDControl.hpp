/*
 * PIDControl.hpp
 *
 *  Created on: 2016/11/21
 *      Author: Keita
 */

#ifndef MIDDLE_INC_PIDCONTROL_HPP_
#define MIDDLE_INC_PIDCONTROL_HPP_

#include <fix.hpp>
#include <unit.hpp>

namespace Middle{
	namespace Controller{
		using namespace common;
		class PID{
		private:
			fix32 mFreq;
			fix32 mGearRate;	// モータ側 /　タイヤ側
			fix32 mRadius;
			fix32 mEncoderResolution;
			fix32 mKp;
			fix32 mKi;
			fix32 mKd;
			fix32 mKe;

			static constexpr fix32 FREQUENCY_DEFAULT = 5_KHz << 16;
			static constexpr fix32 GEAR_RATE_DEFAULT = 1 << 16;
			static constexpr fix32 RADIUS_DEFAULT = 1 << 16;
			static constexpr fix32 ENCODER_RESOLUTION_DEFAULT = 1 << 16;
			static constexpr fix32 K_P_DEFAULT = 1 << 16;
			static constexpr fix32 K_I_DEFAULT = 1 << 16;
			static constexpr fix32 K_D_DEFAULT = 1 << 16;
			static constexpr fix32 K_E_DEFAULT = 1 << 16;

			struct MotorState_t{
				fix32 mError;
				fix32 mIntegration;
				fix32 mTargetSpeed;
				fix32 mRealSpeed;
				fix32 mDuty;

				MotorState_t(): mError(0), mIntegration(0), mTargetSpeed(0), mRealSpeed(0), mDuty(0){}
			};
			MotorState_t mLastState, mNextState;

			void Proc(MotorState_t&, MotorState_t&);
			void SetFreq(fix32 freq);
			void SetFreq(uint32_t freq);
		public:
			PID();
			PID(const PID&) = default;
			virtual ~PID();
			//void Proc();
			void SetGainP(fix32 p);
			void SetGainP(float p);
			void SetGainI(fix32 i);
			void SetGainI(float i);
			void SetGainD(fix32 d);
			void SetGainD(float d);
			void SetGearRate(fix32 g);
			void SetGearRate(uint32_t g);
			void SetTargetSpeed(fix32 speed);
			void SetTargetSpeed(float speed);
			const int32_t GetFreq(){
				return mFreq;
			}

			void operator ()(void){
				Proc(mLastState, mNextState);
			}


		};
	}
}


#endif /* MIDDLE_INC_PIDCONTROL_HPP_ */
