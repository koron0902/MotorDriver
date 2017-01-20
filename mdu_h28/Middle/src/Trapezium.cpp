/*
 * Trapezium.cpp
 *
 *  Created on: 2016/11/28
 *      Author: Keita
 */

#include <Motor.hpp>
#include <Trapezium.hpp>
#include <Port.hpp>

namespace Middle {
	namespace Controller {
		Trapezium::MotorInfo_t Trapezium::mMotorState;

		Trapezium::Trapezium() {
			CallProc = [this](){
				this->Proc(mMotorState);
			};
			//Device::Port::Set(Device::Port::PWMEN, true);
			//Middle::Motor::ModeAs(Middle::Motor::Type::DCMotor);
		}


		Trapezium::~Trapezium() {
			//Device::Port::Set(Device::Port::PWMEN, false);
			//Middle::Motor::ModeAs(Middle::Motor::Type::None);
		}

		void Trapezium::Proc(MotorInfo_t& motor){
			if(motor.mTargetDuty > motor.mLastDuty){
				// 加速時
				if((motor.mLastDuty + motor.mStep) > motor.mTargetDuty){
					// オーバーする時
					motor.mLastDuty = motor.mTargetDuty;
				}else{
					// 通常時
					motor.mLastDuty += motor.mStep;
				}
			}else if(motor.mTargetDuty < motor.mLastDuty){
				// 減速時
				if((motor.mLastDuty - motor.mStep) < motor.mTargetDuty){
					// オーバーする時
					motor.mLastDuty = motor.mTargetDuty;
				}else{
					// 通常時
					motor.mLastDuty -= motor.mStep;
				}
			}

			Motor::SetDuty(motor.mLastDuty);
		}

	} /* namespace Controller */
} /* namespace Middle */
