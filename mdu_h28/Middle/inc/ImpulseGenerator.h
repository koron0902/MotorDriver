/*
 * ImpulseGenerator.h
 *
 *  Created on: 2017/01/30
 *      Author: Keita
 */

#ifndef MIDDLE_INC_IMPULSEGENERATOR_H_
#define MIDDLE_INC_IMPULSEGENERATOR_H_

#include <ControllerBase.hpp>
#include <PIDControl.hpp>
#include <stdint.h>

namespace Middle {
	namespace Controller {

		class ImpulseGenerator: public ControllerBase {
		private:
			PID mPID;
			uint64_t count;
			uint64_t time;
			static bool enable;
		public:
			ImpulseGenerator();
			ImpulseGenerator(const ImpulseGenerator&) = default;
			virtual ~ImpulseGenerator();
			void Generate(float _speed = 0, uint64_t _time = 0);
			static void StartImpulse();
			static void StopImpulse();

			void operator ()(void){
				if(!enable)
					return;

				if(CallProc)
					CallProc();

				if(!enable)
					CallProc = nullptr;
			}
		};

	} /* namespace Controller */
} /* namespace Middle */

#endif /* MIDDLE_INC_IMPULSEGENERATOR_H_ */
