/*
 * Middle.cpp
 *
 *  Created on: 2016/11/12
 *      Author: hamus
 */

#include <motor.hpp>
#include "ff.hpp"
#include "Middle.hpp"
#include "ControllerBase.hpp"
#include <cstring>
#include <HoleSensor.hpp>
namespace Middle {

void Init(){
	HoleSensor::Init();
	Motor::Init();
	Controller::SwitchControlMode(Controller::ControlMode_e::ModePID);
}


}
