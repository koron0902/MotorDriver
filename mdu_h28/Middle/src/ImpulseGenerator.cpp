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
#include <Port.hpp>


namespace Middle {
	namespace Controller {
	
		bool ImpulseGenerator::enable = false;
		PID ImpulseGenerator::mPID;
		uint64_t ImpulseGenerator::count;
		uint64_t ImpulseGenerator::time;
		std::string ImpulseGenerator::buf;
		float ImpulseGenerator::speed;
		std::string ImpulseGenerator::path;
		bool ImpulseGenerator::createCSV = false;

		ImpulseGenerator::ImpulseGenerator(){
			//this->buf.reserve(256);
		}

		ImpulseGenerator::~ImpulseGenerator() {
		}

		void ImpulseGenerator::Generate(float _speed, uint64_t _time, const std::string& _path){
			CallProc = nullptr;
			this->count = 0;
			this->time = _time;
			this->speed = _speed;
			this->path = _path;
			XPort::WriteLine(path);

			this->mPID.SetTargetSpeed(_speed);

			CallProc = [this]()->auto{
				this->count++;
				if(this->count > (this->time * (float)(this->mFreq) / 1000 * 2.0)){
					Device::Port::Set(Device::Port::LED1, false);
					StopImpulse();
					Middle::CSV::Generate(path);
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
			Device::Port::Set(Device::Port::LED1, true);
			Device::Port::Set(Device::Port::LED2, true);
			enable = true;
		}

		void ImpulseGenerator::StopImpulse(){
			enable = false;
		}

	} /* namespace Controller */
} /* namespace Middle */
