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
namespace Middle {

void Init(){
	Controller::SwitchControlMode(Controller::ControlMode_e::ModePID);
	Motor::Init();
}


}
