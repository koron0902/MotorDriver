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
#include <text.hpp>
#include <ControllerBase.hpp>

namespace Middle{
	namespace Controller{
		using namespace common;
		class PID: public ControllerBase{
		private:
			//fix32 mFreq;
			float mGearRate;	// モータ側 /　タイヤ側
			float mRadius;		// タイヤ半径
			float mEncoderResolution;	// エンコーダの分解能 [ppr]
			float mKp;	// 比例ゲイン
			float mKi;	// 積分ゲイン
			float mKd;	// 微分ゲイン
			float mKe;	// 逆起電力ゲイン

			static constexpr float FREQUENCY_DEFAULT = 10_KHz;
			static constexpr float GEAR_RATE_DEFAULT = 1;
			static constexpr float RADIUS_DEFAULT = 1;
			static constexpr float ENCODER_RESOLUTION_DEFAULT = 4000;
			static constexpr float K_P_DEFAULT = 0.02819;
			static constexpr float K_I_DEFAULT = 0.00242;
			static constexpr float K_D_DEFAULT = 0;
			static constexpr float K_E_DEFAULT = 1 / 6.23333;
public:
			struct MotorState_t{
				float mError;		// 偏差
				float mIntegration;	// 積分値
				float mTargetSpeed;	// [rps]
				float mRealSpeed;	// [rps]
				float mDuty;

				MotorState_t(): mError(0), mIntegration(0), mTargetSpeed(0), mRealSpeed(0), mDuty(0){}
			};
private:
			static MotorState_t mNextState, mLastState;

			void Proc(MotorState_t&, MotorState_t&);
			void SetFreq(fix32 freq);
			void SetFreq(uint32_t freq);
		public:
			static fix32 LastDuty;
			PID();
			PID(const PID&) = default;
			virtual ~PID();

			static const void Reset(){
				MotorState_t reset;
				mNextState = mLastState = reset;
			}

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
		};
	}
}


#endif /* MIDDLE_INC_PIDCONTROL_HPP_ */
