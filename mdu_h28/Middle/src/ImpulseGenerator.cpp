/*
 * ImpulseGenerator.cpp
 *
 *  Created on: 2017/01/30
 *      Author: Keita
 */

#include <ImpulseGenerator.h>
#include <xport.hpp>
#include <text.hpp>
#include <CSVGenerator.hpp>


namespace Middle {
	namespace Controller {
	
		bool ImpulseGenerator::enable = false;

		ImpulseGenerator::ImpulseGenerator(){
			this->buf.reserve(256);
		}

		ImpulseGenerator::~ImpulseGenerator() {
		}

		void ImpulseGenerator::Generate(float _speed, uint64_t _time){
			CallProc = nullptr;
			this->count = 0;
			this->time = _time;
			this->speed = _speed;

			this->mPID.SetTargetSpeed(_speed);
			Middle::CSV::Init();

			CallProc = [this]()->auto{
				this->count++;
				if(this->count > (this->time * (float)(this->mFreq) / 1000 * 3.0)){
					StopImpulse();
					Middle::CSV::Generate("Impulse");
				}else if((this->count) > (this->time * (float)(this->mFreq) / 1000)){
					this->mPID.SetTargetSpeed(0);
					this->speed = 0;
				}
				this->mPID();
				this->buf = common::ToStr(this->count) + "," + common::ToStr(this->speed) + "," + this->mPID.GetSpeed() + "\n";
				if(this->count % (this->time / 100) == 0)
					Middle::CSV::Add2Buf(this->buf);
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
