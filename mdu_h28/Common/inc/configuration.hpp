/*
 * configuration.h
 *
 *  Created on: 2017/01/20
 *      Author: hamus
 */

#ifndef COMMON_INC_CONFIGURATION_HPP_
#define COMMON_INC_CONFIGURATION_HPP_

#include <stdint.h>

namespace common {

//割り込み優先度に関して
//未分類
extern const uint8_t PriorityMRT;
extern const uint8_t PriorityADC;

//システム系
extern const uint8_t PriorityDRVFault;
extern const uint8_t PrioritySysTick;
//角度制御
extern const uint8_t PriorityHoleSensor;
//周期制御
extern const uint8_t PriorityCycle;

//通信系
extern const uint8_t PriorityUart;
extern const uint8_t PriorityUSB;


}

#endif /* COMMON_INC_CONFIGURATION_HPP_ */
