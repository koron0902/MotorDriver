/*
 * Trapezium.hpp
 *
 *  Created on: 2016/11/28
 *      Author: Keita
 */

#ifndef MIDDLE_INC_TRAPEZIUM_HPP_
#define MIDDLE_INC_TRAPEZIUM_HPP_

#include <ControllerBase.hpp>
#include <stdint.h>
#include <fix.hpp>
#include <unit.hpp>
#include <text.hpp>

namespace Middle {
	namespace Controller {
		using namespace common;
		class Trapezium: public ControllerBase {
		private:
			typedef struct MotorInfo_t{
				fix32 mLastDuty;
				fix32 mTargetDuty;
				fix32 mStep;
				MotorInfo_t(): mLastDuty(0), mTargetDuty(0), mStep(STEP_DEFAULT){}
			}MotorInfo_t;
			static MotorInfo_t mMotorState;

			void Proc(MotorInfo_t& motor);

		public:
			Trapezium();
			Trapezium(const Trapezium&) = default;
			virtual ~Trapezium();


			/*void operator ()(void){
				Proc(mMotorState);
			};*/

			static const std::string SetTargetDuty(const std::string& duty){
				mMotorState.mTargetDuty = common::ToFix(duty);
				return "";
			}

			static const std::string GetNowDuty(){
				return(common::ToStr(mMotorState.mLastDuty));
			}

			static const std::string SetStep(const std::string& step){
				mMotorState.mStep = common::ToFix(step);

				return "";
			}

			static const std::string GetStep(){
				return (common::ToStr(mMotorState.mStep));
			}
		};
	}
}

#endif /* MIDDLE_INC_TRAPEZIUM_HPP_ */
