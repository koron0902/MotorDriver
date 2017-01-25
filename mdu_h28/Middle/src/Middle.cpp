/*
 * Middle.cpp
 *
 *  Created on: 2016/11/12
 *      Author: hamus
 */

#include "ff.hpp"
#include "Middle.hpp"
#include "ControllerBase.hpp"
#include <cstring>
#include <HoleSensor.hpp>
#include <Motor.hpp>
namespace Middle {

void Init(){

	Motor::Init();
	Motor::SwitchMotorType(Motor::MotorType::BLDCWithSensor);
	Controller::SwitchControlMode(Controller::ControlMode::None);
}


}
