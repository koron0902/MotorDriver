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
			static float mGearRate;	// モータ側 /　タイヤ側
			static float mRadius;		// タイヤ半径
			static float mEncoderResolution;	// エンコーダの分解能 [ppr]
			static float mKp;	// 比例ゲイン
			static float mKi;	// 積分ゲイン
			static float mKd;	// 微分ゲイン
			static float mKe;	// 逆起電力ゲイン

			static constexpr float FREQUENCY_DEFAULT = 1_KHz;
			static constexpr float GEAR_RATE_DEFAULT = 1;
			static constexpr float RADIUS_DEFAULT = 1;
			static constexpr float ENCODER_RESOLUTION_DEFAULT = 4090;
			static constexpr float K_P_DEFAULT = 0.0987;
			static constexpr float K_I_DEFAULT = 0.00041;
			static constexpr float K_D_DEFAULT = 0.001;
			static constexpr float K_E_DEFAULT = 60 / 285;	// モータの速度定数の逆数[V/rps]
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

			static void SetGainP(float p);
			static void SetGainI(float i);
			static void SetGainD(float d);
			static void SetGearRate(float g);
			static void SetTargetSpeed(float speed);
			static std::string GetGainP();
			static std::string GetGainI();
			static std::string GetGainD();
			static std::string GetGearRate();
			static std::string GetSpeed();
		};
	}
}


#endif /* MIDDLE_INC_PIDCONTROL_HPP_ */
