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

			void Proc(MotorInfo_t& motor);

		public:
			static MotorInfo_t mMotorState;

			Trapezium();
			Trapezium(const Trapezium&) = default;
			virtual ~Trapezium();
			static const void Reset(){
				MotorInfo_t reset;
				mMotorState = reset;
			}
		};
	}
}

#endif /* MIDDLE_INC_TRAPEZIUM_HPP_ */
