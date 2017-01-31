/*
 * ImpulseGenerator.cpp
 *
 *  Created on: 2017/01/30
 *      Author: Keita
 */

#include <ImpulseGenerator.h>
#include <xport.hpp>
#include <text.hpp>


namespace Middle {
	namespace Controller {
	
		bool ImpulseGenerator::enable = false;

		ImpulseGenerator::ImpulseGenerator(){
		}

		ImpulseGenerator::~ImpulseGenerator() {
		}

		void ImpulseGenerator::Generate(float _speed, uint64_t _time){
			CallProc = nullptr;
			this->count = 0;
			this->time = _time;

			this->mPID.SetTargetSpeed(_speed);

			CallProc = [this]()->auto{
				this->count++;
				if(this->count > (this->time * (float)(this->mFreq) / 1000 * 3.0)){
					StopImpulse();
				}else if((this->count) > (this->time * (float)(this->mFreq) / 1000)){
					this->mPID.SetTargetSpeed(0);
				}
				this->mPID();
			};
		}

		void ImpulseGenerator::StartImpulse(){
			enable = true;
		}

		void ImpulseGenerator::StopImpulse(){
			enable = false;
		}

	} /* namespace Controller */
} /* namespace Middle */
