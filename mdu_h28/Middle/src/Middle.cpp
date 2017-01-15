/*
 * Middle.cpp
 *
 *  Created on: 2016/11/12
 *      Author: hamus
 */

#include <motor.hpp>
#include <Middle.hpp>
#include "ff.hpp"
#include "Middle.hpp"
#include "ControllerBase.hpp"

namespace Middle {

void Init(){
	Controller::SwitchControlMode(Controller::ControlMode_e::ModePID);
	Motor::Init();
	uint8_t *work;
	work = new uint8_t[512];
	FatFs::f_mkfs("0", (FM_FAT | FM_SFD), 0, work, 512);
	delete[] work;
	
}


}
