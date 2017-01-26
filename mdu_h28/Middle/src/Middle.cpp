/*
 * Middle.cpp
 *
 *  Created on: 2016/11/12
 *      Author: hamus
 */

#include <Control.hpp>
#include "ff.hpp"
#include "Middle.hpp"
#include <cstring>
#include <HoleSensor.hpp>
#include <Motor.hpp>
namespace Middle {

void Init(){
	//各機能の初期化
	Motor::Init();
	Control::Init();


	//実験用コード
	Motor::SwitchMotorType(Motor::MotorType::BLDCWithSensor);

}


}
