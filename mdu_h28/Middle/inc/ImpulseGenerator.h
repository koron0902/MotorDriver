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
			static PID mPID;
			static uint64_t count;
			static uint64_t time;
			static std::string buf;
			static float speed;
			static std::string path;
			static bool createCSV;
		public:
			static bool enable;
			ImpulseGenerator();
			ImpulseGenerator(const ImpulseGenerator&) = default;
			virtual ~ImpulseGenerator();
			void Dummy(){
				this->Generate(speed, time, path);
			}
			void Generate(float _speed = 0, uint64_t _time = 0, const std::string& _path = "impulse");
			static void StartImpulse();
			static void StopImpulse();

			void operator ()(void){
				if(!enable && count == 0)
					return;
				else if(!enable && count != 0){
					CallProc = nullptr;
				}

				if(CallProc)
					CallProc();
			}
		};

	} /* namespace Controller */
} /* namespace Middle */

#endif /* MIDDLE_INC_IMPULSEGENERATOR_H_ */
