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
#include <CSVGenerator.hpp>
#include <string>
#include <xport.hpp>

namespace Middle {
	namespace Controller {

		class ImpulseGenerator: public ControllerBase {
		private:
			PID mPID;
			uint64_t count;
			uint64_t time;
			std::string buf;
			float speed;
		public:
			static bool enable;
			ImpulseGenerator();
			ImpulseGenerator(const ImpulseGenerator&) = default;
			virtual ~ImpulseGenerator();
			void Generate(float _speed = 0, uint64_t _time = 0);
			static void StartImpulse();
			static void StopImpulse();

			void operator ()(void){
				if(!enable && count == 0)
					return;
				else if(!enable && count != 0)
					CallProc = nullptr;

				if(CallProc)
					CallProc();
			}
		};

	} /* namespace Controller */
} /* namespace Middle */

#endif /* MIDDLE_INC_IMPULSEGENERATOR_H_ */
